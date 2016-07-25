
# SYSTEM IMPORTS
import os
import sys


currentDir = os.path.dirname(os.path.realpath(__file__))
buildScriptsDir = os.path.join(currentDir, "build", "scripts")


def urljoin(url, *urls):
    urlList = [url]
    urlList.extend([urlPart for urlPart in urls])
    unrefinedUrl = '/'.join(urlList).strip()
    unrefinedUrl = unrefinedUrl.replace("//", "/")
    return unrefinedUrl.replace("http:/", "http://")


def downloadBuildScripts():
    import pymongo
    import requests
    import tarfile

    downloadedBuildScriptsPath = os.path.join(buildScriptsDir, "Scripts_download")
    if not os.path.exists(downloadedBuildScriptsPath):
        os.makedirs(downloadedBuildScriptsPath)
    client = pymongo.MongoClient(os.environ["MONGODB_URI"])
    db = client["BuildScripts"]
    coll = db["src"]
    mostRecentBuildScriptsRecord = [x for x in coll.find(
        {
            "config": "src"
        }
    ).sort("build_num")][-1]
    filePath = os.path.join(downloadedBuildScriptsPath,
                            mostRecentBuildScriptsRecord["filename"] + mostRecentBuildScriptsRecord["filetype"])
    response = requests.get(urljoin(os.environ["FILESERVER_URI"], "BuildScripts/"), stream=True)
    with open(filePath, "wb") as f:
        for chunk in response.iter_content():
            f.write(chunk)
    with tarfile.open(filePath, "r:gz") as tarFile:
        # extract out scripts and config dirs and move it to currentDir/build
        tarFile.extractAll(path=os.path.join(currentDir, "build"), members=["scripts", "config"])


if not os.path.exists(buildScriptsDir):
    os.makedirs(buildScriptsDir)
    downloadBuildScripts()
sys.path.extend([currentDir, buildScriptsDir])  # now we can import modules from <currentDirectory>/scripts


# PYTHON PROJECT IMPORTS
import ProjectBuild
import Utilities
import FileSystem


class LocalBuild(ProjectBuild.ProjectBuild):
    def __init__(self):
        super(LocalBuild, self).__init__("Async")
        self._build_steps = [self.build,
                             self.runUnitTests,
                             self.package,
                             self.uploadPackagedVersion]

    def generateConfig(self, asyncConfigPath=None, asyncConfigFileName=None):
        outIncludeDir = os.path.join(FileSystem.getDirectory(FileSystem.OUT_ROOT),
                                     "include")
        projectLogDir = FileSystem.getDirectory(FileSystem.LOG_DIR, self._config, self._project_name)
        asyncConfig = None
        if asyncConfigPath is None:
            asyncConfig = os.path.join(FileSystem.getDirectory(FileSystem.CLIENT_CONFIG),
                                       (asyncConfigFileName if asyncConfigFileName is not None else "AsyncConfig.xml"))
        else:
            asyncConfig = asyncConfigPath
        Utilities.mkdir(outIncludeDir)

        configArgs = []

        configArgs.append(['std::string', 'LOGGING_ROOT', 'dir', projectLogDir.replace("\\", "/")])
        if "Robos" in self._project_name:
            configArgs.append(['std::string', 'ASYNC_CONFIG_PATH', 'file', asyncConfig.replace("\\", "/")])

        (formattedConfigArgsHeader, formattedConfigArgsSrc) = self.checkConfigArgsAndFormat("\t", configArgs)

        if os.path.exists(projectLogDir):
            Utilities.rmTree(projectLogDir)
        Utilities.mkdir(projectLogDir)
        projNameUpper = self._project_name.upper()
        with open(os.path.join(outIncludeDir, self._project_name + "Config.hpp"), 'w') as file:
            file.write("#pragma once\n"
                       "#ifndef " + projNameUpper + "_CONFIG_" + projNameUpper + "CONFIG_HPP\n"
                       "#define " + projNameUpper + "_CONFIG_" + projNameUpper + "CONFIG_HPP\n\n"
                       "// SYSTEM INCLUDES\n"
                       "#include <string>\n\n"
                       "// C++ PROJECT INCLUDES\n\n"
                       "namespace " + self._project_name + "\n"
                       "{\n"
                       "namespace Config\n"
                       "{\n\n" +
                       formattedConfigArgsHeader +
                       "} // end of namespace Config\n"
                       "} // end of namespace " + self._project_name + "\n"
                       "#endif // end of " + projNameUpper + "_CONFIG_" + projNameUpper + "CONFIG_HPP\n")
        with open(os.path.join(outIncludeDir, self._project_name + "Config.cpp"), 'w') as file:
            file.write("// SYSTEM INCLUDES\n\n"
                       "// C++ PROJECT INCLUDES\n"
                       "#include \"" + self._project_name + "Config.hpp\"\n\n"
                       "namespace " + self._project_name + "\n"
                       "{\n"
                       "namespace Config\n"
                       "{\n\n" +
                       formattedConfigArgsSrc +
                       "} // end of namespace Config\n"
                       "} // end of namespace " + self._project_name + "\n")

    def preBuild(self, asyncConfigPath=None, asyncConfigFileName=None):
        self.setupWorkspace()
        self.generateProjectVersion()
        self.generateConfig(asyncConfigPath, asyncConfigFileName)

    def help(self):
        super(LocalBuild, self).help()

if __name__ == "__main__":
    customCommands = Utilities.parseCommandLine(sys.argv[1:])
    print(customCommands)

    localBuild = LocalBuild()
    if "help" in customCommands:
        localBuild.help()
    else:
        localBuild.run(customCommands)
