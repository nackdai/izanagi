﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.VisualStudio.VCProjectEngine;
using MakeItSoLib;
using System.IO;
using System.Reflection;

namespace SolutionParser_VS2010
{
    /// <summary>
    /// Parses a C++ project.
    /// </summary><remarks>
    /// We extract information from a VCProject object, and fill in a  ProjectInfo structure.
    /// </remarks>
    internal class ProjectParser_CPP
    {
        #region Public methods and properties

        /// <summary>
        /// Constructor
        /// </summary>
        public ProjectParser_CPP(VCProject vcProject, string solutionRootFolder)
        {
            try
            {
                m_vcProject = vcProject;
                m_solutionRootFolder = solutionRootFolder;

                // We get the project name...
                m_projectInfo.Name = Utils.call(() => (m_vcProject.Name));
                
                if (MakeItSoConfig.Instance.ignoreProject(m_projectInfo.Name) == true)
                    Log.log("- project " + m_projectInfo.Name + "ignored");
                else
                {
                    Log.log("- parsing project " + m_projectInfo.Name);

                    // and parse the project...
                    parseProject();
                    Log.log("  - done");
                }
            }
            catch (Exception ex)
            {
                Log.log(String.Format("  - FAILED ({0})", ex.Message));
            }
        }

        /// <summary>
        /// Gets the parsed project.
        /// </summary>
        public ProjectInfo_CPP Project
        {
            get { return m_projectInfo; }
        }

        #endregion

        #region Private functions

        /// <summary>
        /// Parses the project.
        /// </summary>
        private void parseProject()
        {
            parseProject_RootFolder();
            parseProject_SourceFiles();
            parseProject_Configurations();
        }

        /// <summary>
        /// Finds the configurations, e.g. debug, release etc.
        /// </summary>
        private void parseProject_Configurations()
        {
            MakeItSoConfig_Project projectSettings = MakeItSoConfig.Instance.getProjectConfig(m_projectInfo.Name);

            // We loop through the collection of configurations for the project...
            IVCCollection configurations = Utils.call(() => (m_vcProject.Configurations as IVCCollection));
            int numConfigurations = Utils.call(() => (configurations.Count));
            for (int i = 1; i <= numConfigurations; ++i)
            {
                // We parse this configuration, and add the parsed data to the collection
                // for this project...
                VCConfiguration vcConfiguration = Utils.call(() => (configurations.Item(i) as VCConfiguration));

                if (projectSettings.configurationShouldBeRemoved(vcConfiguration.Name) == false)
                {
                    parseConfiguration(vcConfiguration);
                }
            }
        }

        /// <summary>
        /// Parses the configuration (e.g. Debug, Release) passed in.
        /// </summary>
        private void parseConfiguration(VCConfiguration vcConfiguration)
        {
            ProjectConfigurationInfo_CPP configurationInfo = new ProjectConfigurationInfo_CPP();
            configurationInfo.ParentProjectInfo = m_projectInfo;

            // The configuration name...
            configurationInfo.Name = Utils.call(() => (vcConfiguration.ConfigurationName));

            // The project type. 
            // Note: we are assuming that all the configurations for the project build the
            //       same type of target. 
            m_projectInfo.ProjectType = parseConfiguration_Type(vcConfiguration);

            // We get the intermediates and output folder, and the Target Name.
            configurationInfo.IntermediateFolder = parseConfiguration_Folder(vcConfiguration, () => (vcConfiguration.IntermediateDirectory));
            configurationInfo.OutputFolder = parseConfiguration_Folder(vcConfiguration, () => (vcConfiguration.OutputDirectory));
            configurationInfo.TargetName = parseConfiguration_TargetName(vcConfiguration);

            // We get compiler settings, such as the include path and 
            // preprocessor definitions...
            parseConfiguration_CompilerSettings(vcConfiguration, configurationInfo);

            // We get linker settings, such as any libs to link and the library path...
            parseConfiguration_LinkerSettings(vcConfiguration, configurationInfo);

            // We parse librarian settings (how libraries are linked)...
            parseConfiguration_LibrarianSettings(vcConfiguration, configurationInfo);

            // We see if there are any pre- or post- build events set up for this
            // configuration. The only types of events we know how to deal with are
            // ones that invoke a .cmd file. For these we convert them to invoke
            // a .sh file with the same name.
            parseConfiguration_PreBuildEvent(vcConfiguration, configurationInfo);
            parseConfiguration_PostBuildEvent(vcConfiguration, configurationInfo);

            // We add the configuration to the collection of them for the project...
            m_projectInfo.addConfigurationInfo(configurationInfo);
        }

        /// <summary>
        /// We parse the pre-build events.
        /// </summary>
        private void parseConfiguration_PreBuildEvent(VCConfiguration vcConfiguration, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // We see if there is a pre-build event...
            IVCCollection tools = Utils.call(() => (vcConfiguration.Tools as IVCCollection));
            VCPreBuildEventTool preBuildEvent = Utils.call(() => (tools.Item("VCPreBuildEventTool") as VCPreBuildEventTool));
            if (preBuildEvent == null)
            {
                return;
            }
            string commandLine = Utils.call(() => (preBuildEvent.CommandLine));
            configurationInfo.PreBuildEvent = convertBuildEventCommandLine(commandLine);
        }

        /// <summary>
        /// We parse the post-build events.
        /// </summary>
        private void parseConfiguration_PostBuildEvent(VCConfiguration vcConfiguration, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // We see if there is a pre-build event...
            IVCCollection tools = Utils.call(() => (vcConfiguration.Tools as IVCCollection));
            VCPostBuildEventTool postBuildEvent = Utils.call(() => (tools.Item("VCPostBuildEventTool") as VCPostBuildEventTool));
            if (postBuildEvent == null)
            {
                return;
            }
            string commandLine = Utils.call(() => (postBuildEvent.CommandLine));
            configurationInfo.PostBuildEvent = convertBuildEventCommandLine(commandLine);
        }

        /// <summary>
        /// Converts the build-step command-line to one we can run in Linux.
        /// </summary>
        private string convertBuildEventCommandLine(string commandLine)
        {
            if (commandLine == null)
            {
                return "";
            }

            // We only convert a command line if it invokes a .cmd file...
            commandLine = commandLine.Trim();
            if (commandLine.EndsWith(".cmd") == false)
            {
                Console.WriteLine("Build event does not invoke a .cmd file. MakeItSo only converts [event].cmd -> [event].sh");
                return "";
            }

            // We've got an event that runs a .cmd file. So we replace the extension
            // with .sh and convert the path to Linux format...
            commandLine = Path.ChangeExtension(commandLine, ".sh");
            commandLine = Utils.makeRelativePath(m_projectInfo.RootFolderAbsolute, commandLine);

            // We check if the file in in the root folder. If so, we need to change its
            // name to ./[name.sh] for Linux...
            if (commandLine.Contains("/") == false)
            {
                commandLine = "./" + commandLine;
            }

            return commandLine;
        }

        /// <summary>
        /// We parse the librarian settings, ie link options for libraries.
        /// </summary>
        private void parseConfiguration_LibrarianSettings(VCConfiguration vcConfiguration, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // We get the librarian 'tool'...
            IVCCollection tools = Utils.call(() => (vcConfiguration.Tools as IVCCollection));
            VCLibrarianTool librarianTool = Utils.call(() => (tools.Item("VCLibrarianTool") as VCLibrarianTool));
            if (librarianTool == null)
            {
                // Not all projects have a librarian tool...
                return;
            }

            // We find if this library is set to link together other libraries it depends on...
            // (We are assuming that all configurations of the project have the same link-library-dependencies setting.)
            m_projectInfo.LinkLibraryDependencies = Utils.call(() => (librarianTool.LinkLibraryDependencies));
        }

        /// <summary>
        /// Finds the linker settings, such as the collection of libraries to link,
        /// for the configuration passed in.
        /// </summary>
        private void parseConfiguration_LinkerSettings(VCConfiguration vcConfiguration, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // We get the linker-settings 'tool'...
            IVCCollection tools = Utils.call(() => (vcConfiguration.Tools as IVCCollection));
            VCLinkerTool linkerTool = Utils.call(() => (tools.Item("VCLinkerTool") as VCLinkerTool));
            if (linkerTool == null)
            {
                // Not all projects have a linker tools...
                return;
            }

            // And extract various details from it...
            parseLinkerSettings_LibraryPath(vcConfiguration, linkerTool, configurationInfo);
            parseLinkerSettings_Libraries(vcConfiguration, linkerTool, configurationInfo);
            parseLinkerSettings_Misc(vcConfiguration, linkerTool, configurationInfo);
        }

        /// <summary>
        /// Reads miscellaneous linker settings.
        /// </summary>
        private void parseLinkerSettings_Misc(VCConfiguration vcConfiguration, VCLinkerTool linkerTool, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // Whether we implicitly link in libraries we depend on.
            // (We are assuming that all configurations of the project have the
            // same link-library-dependencies setting.)
            m_projectInfo.LinkLibraryDependencies = Utils.call(() => (linkerTool.LinkLibraryDependencies));

            // Generate debug info...
            bool debugInfo = Utils.call(() => (linkerTool.GenerateDebugInformation));
            if (debugInfo == true
                &&
                configurationInfo.getPreprocessorDefinitions().Contains("NDEBUG") == false)
            {
                configurationInfo.addCompilerFlag("-g");
            }
        }

        /// <summary>
        /// Finds the library path for the configuration passed in.
        /// </summary>
        private void parseLinkerSettings_LibraryPath(VCConfiguration vcConfiguration, VCLinkerTool linkerTool, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // We:
            // 1. Read the additional library paths (which are in a semi-colon-delimited string)
            // 2. Split it into separate paths
            // 3. Resolve any symbols
            // 4. Make sure all paths are relative to the project root folder

            // 1 & 2...
            string strAdditionalLibraryDirectories = Utils.call(() => (linkerTool.AdditionalLibraryDirectories));
            if (strAdditionalLibraryDirectories == null)
            {
                return;
            }

            List<string> additionalLibraryDirectories = Utils.split(strAdditionalLibraryDirectories, ';', ',');
            foreach (string additionalLibraryDirectory in additionalLibraryDirectories)
            {
                // The string may be quoted. We need to remove the quotes...
                string unquotedLibraryDirectory = additionalLibraryDirectory.Trim('"');
                if (unquotedLibraryDirectory == "")
                {
                    continue;
                }

                // 3 & 4...
                string resolvedPath = Utils.call(() => (vcConfiguration.Evaluate(unquotedLibraryDirectory)));
                if (resolvedPath != "")
                {
                    string relativePath = Utils.makeRelativePath(m_projectInfo.RootFolderAbsolute, resolvedPath);
                    configurationInfo.addLibraryPath(relativePath);
                }
            }
        }

        /// <summary>
        /// Finds the collection of additional libraries to link into this project.
        /// </summary>
        private void parseLinkerSettings_Libraries(VCConfiguration vcConfiguration, VCLinkerTool linkerTool, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // The collection of libraries is stored in a space-delimited string...
            string strAdditionalLibraries = Utils.call(() => (linkerTool.AdditionalDependencies));
            if (strAdditionalLibraries == null)
            {
                return;
            }

            List<string> additionalLibraries = Utils.split(strAdditionalLibraries, ' ');
            foreach (string additionalLibrary in additionalLibraries)
            {
                // We add the library to the project...
                string rawName = Path.GetFileNameWithoutExtension(additionalLibrary);
                configurationInfo.addLibraryRawName(rawName);
            }
        }

        /// <summary>
        /// Finds compiler settings, such as the include path, for the configuration
        /// passed in.
        /// </summary>
        private void parseConfiguration_CompilerSettings(VCConfiguration vcConfiguration, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // We get the compiler-settings 'tool'...
            IVCCollection tools = Utils.call(() => (vcConfiguration.Tools as IVCCollection));
            VCCLCompilerTool compilerTool = Utils.call(() => (tools.Item("VCCLCompilerTool") as VCCLCompilerTool));

            // And extract various details from it...
            parseCompilerSettings_IncludePath(vcConfiguration, compilerTool, configurationInfo);
            parseCompilerSettings_PreprocessorDefinitions(vcConfiguration, compilerTool, configurationInfo);
            parseCompilerSettings_CompilerFlags(vcConfiguration, compilerTool, configurationInfo);

            IVCCollection sheets = Utils.call(() => (vcConfiguration.PropertySheets as IVCCollection));
            int numSheets = Utils.call(() => (sheets.Count));
            for (int i = 1; i <= numSheets; ++i)
            {
                VCPropertySheet sheet = Utils.call(() => (sheets.Item(i) as VCPropertySheet));

                if (!sheet.IsSystemPropertySheet)
                {
                    // 1. The thing is that VCPropertySheet and VCConfiguration have more-or-less
                    //    identical interfaces. So we should be able to merge them fairly easily.
                    //
                    // 2. We should try multiple layers of inheritance

                    IVCCollection toolsInSheet = Utils.call(() => (sheet.Tools as IVCCollection));
                    VCCLCompilerTool compilerToolInSheet = Utils.call(() => (toolsInSheet.Item("VCCLCompilerTool") as VCCLCompilerTool));

                    // And extract various details from it...
                    parseCompilerSettings_IncludePath(vcConfiguration, compilerToolInSheet, configurationInfo);
                    parseCompilerSettings_PreprocessorDefinitions(vcConfiguration, compilerToolInSheet, configurationInfo);
                    parseCompilerSettings_CompilerFlags(vcConfiguration, compilerToolInSheet, configurationInfo);
                }
            }
        }

        /// <summary>
        /// Finds compiler flags.
        /// </summary>
        private void parseCompilerSettings_CompilerFlags(VCConfiguration vcConfiguration, VCCLCompilerTool compilerTool, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // Warning level...
            warningLevelOption warningLevel = Utils.call(() => (compilerTool.WarningLevel));
            switch (warningLevel)
            {
                case warningLevelOption.warningLevel_0:
                    configurationInfo.addCompilerFlag("-w");
                    break;

                case warningLevelOption.warningLevel_4:
                    configurationInfo.addCompilerFlag("-Wall");
                    break;
            }

            // Warnings as errors...
            bool warningsAsErrors = Utils.call(() => (compilerTool.WarnAsError));
            if (warningsAsErrors == true)
            {
                configurationInfo.addCompilerFlag("-Werror");
            }

            // Optimization...
            optimizeOption optimization = Utils.call(() => (compilerTool.Optimization));
            switch (optimization)
            {
                case optimizeOption.optimizeDisabled:
                    configurationInfo.addCompilerFlag("-O0");
                    break;

                case optimizeOption.optimizeMinSpace:
                    configurationInfo.addCompilerFlag("-Os");
                    break;

                case optimizeOption.optimizeMaxSpeed:
                    configurationInfo.addCompilerFlag("-O2");
                    break;

                case optimizeOption.optimizeFull:
                    configurationInfo.addCompilerFlag("-O3");
                    break;
            }
        }

        /// <summary>
        /// Finds the collection of include paths for the configuration passed in.
        /// </summary>
        private void parseCompilerSettings_IncludePath(VCConfiguration vcConfiguration, VCCLCompilerTool compilerTool, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // We:
            // 1. Read the additional include paths (which are in a semi-colon-delimited string)
            // 2. Split it into separate paths
            // 3. Resolve any symbols
            // 4. Make sure all paths are relative to the project root folder

            // 1 & 2...
            string strAdditionalIncludeDirectories = Utils.call(() => (compilerTool.AdditionalIncludeDirectories));
            if (strAdditionalIncludeDirectories == null)
            {
                return;
            }

            List<string> additionalIncludeDirectories = Utils.split(strAdditionalIncludeDirectories, ';', ',');
            foreach (string additionalIncludeDirectory in additionalIncludeDirectories)
            {
                // The string may be quoted. We need to remove the quotes...
                string unquotedIncludeDirectory = additionalIncludeDirectory.Trim('"');

                // 3 & 4...
                string resolvedPath = Utils.call(() => (vcConfiguration.Evaluate(unquotedIncludeDirectory)));
                if (resolvedPath != "")
                {
                    string relativePath = Utils.makeRelativePath(m_projectInfo.RootFolderAbsolute, resolvedPath);
                    configurationInfo.addIncludePath(relativePath);
                }
            }
        }

        /// <summary>
        /// Finds the collection of preprocessor definitions for the configuration passed in.
        /// </summary>
        private void parseCompilerSettings_PreprocessorDefinitions(VCConfiguration vcConfiguration, VCCLCompilerTool compilerTool, ProjectConfigurationInfo_CPP configurationInfo)
        {
            // We read the delimited string of preprocessor definitions, and
            // split them...
            string strPreprocessorDefinitions = Utils.call(() => (compilerTool.PreprocessorDefinitions));
            if (strPreprocessorDefinitions == null)
            {
                return;
            }
            List<string> preprocessorDefinitions = Utils.split(strPreprocessorDefinitions, ';');

            // We add the definitions to the parsed configuration (removing ones that 
            // aren't relevant to a linux build)...
            foreach (string definition in preprocessorDefinitions)
            {
                configurationInfo.addPreprocessorDefinition(definition);
            }
        }

        /// <summary>
        /// Gets the configuration type.
        /// </summary>
        private ProjectInfo.ProjectTypeEnum parseConfiguration_Type(VCConfiguration vcConfiguration)
        {
            ProjectInfo.ProjectTypeEnum result = ProjectInfo.ProjectTypeEnum.INVALID;

            // We get the Visual Studio confiuration type...
            ConfigurationTypes configurationType = Utils.call(() => (vcConfiguration.ConfigurationType));

            // And convert it to our enum type...
            switch (configurationType)
            {
                case ConfigurationTypes.typeApplication:
                    result = ProjectInfo.ProjectTypeEnum.CPP_EXECUTABLE;
                    break;

                case ConfigurationTypes.typeStaticLibrary:
                    result = ProjectInfo.ProjectTypeEnum.CPP_STATIC_LIBRARY;
                    break;

                case ConfigurationTypes.typeDynamicLibrary:
                    result = ProjectInfo.ProjectTypeEnum.CPP_DLL;
                    break;
            }

            return result;
        }

        /// <summary>
        /// Gets a folder name from the function passed in, and returns it as a path
        /// relative to the project root folder.
        /// </summary>
        private string parseConfiguration_Folder(VCConfiguration vcConfiguration, Func<string> folderFn)
        {
            // We get the folder name, which may contain symbols e.g. £(ConfgurationName)...
            string pathWithSymbols = Utils.call(folderFn);

            // We resolve the symbols...
            string evaluatedPath = Utils.call(() => (vcConfiguration.Evaluate(pathWithSymbols)));

            // If we ave an absolute path, we convert it to a relative one...
            string relativePath = evaluatedPath;
            if (Path.IsPathRooted(evaluatedPath))
            {
                relativePath = Utils.makeRelativePath(m_projectInfo.RootFolderAbsolute, evaluatedPath);
            }

            return relativePath;
        }
		
		/// <summary>
		/// Gets the TargetName from the project and returns it
		/// </summary>
        private string parseConfiguration_TargetName(VCConfiguration vcConfiguration)
        {
            string[] PrimaryOutput = vcConfiguration.PrimaryOutput.Split('\\');
            string[] TargetName = PrimaryOutput[PrimaryOutput.Length - 1].Split('.');
         
            return TargetName[0];
        }

        /// <summary>
        /// Finds the project's root folder.
        /// </summary>
        private void parseProject_RootFolder()
        {
            // The project root folder, both absolute and relative to the solution root...
            m_projectInfo.RootFolderAbsolute = Utils.call(() => (m_vcProject.ProjectDirectory));
            m_projectInfo.RootFolderRelative = Utils.makeRelativePath(m_solutionRootFolder, m_projectInfo.RootFolderAbsolute);
        }

        /// <summary>
        /// Finds the collection of source files in the project.
        /// </summary>
        private void parseProject_SourceFiles()
        {
            // We loop through the collection of files in the project...
            IVCCollection files = Utils.call(() => (m_vcProject.Files as IVCCollection));
            int numFiles = Utils.call(() => (files.Count));
            for (int i = 1; i <= numFiles; ++i)
            {
                // We get one file...
                VCFile file = Utils.call(() => (files.Item(i) as VCFile));
                string path = Utils.call(() => (file.FullPath));
                string extension = Path.GetExtension(path).ToLower();

                // We check if the file has a custom build rule...
                parseCustomBuildRule(file);

                switch (extension)
                {
                    // It looks like a source file...
                    case ".cpp":
                    case ".c":
                    case ".cc":
                    case ".cp":
                    case ".cxx":
                    case ".c++":
                        // We add it to the project...
                        string relativePath = Utils.makeRelativePath(m_projectInfo.RootFolderAbsolute, path);
                        m_projectInfo.addFile(relativePath);
                        break;
                }
            }
        }

        /// <summary>
        /// Checks the file passes in to see if it has a custom build rule set up,
        /// and parses it if it does.
        /// </summary>
        private void parseCustomBuildRule(VCFile file)
        {
            // We find the relative path to the file from the project root...
            string path = Utils.call(() => (file.FullPath));
            string relativePath = Utils.makeRelativePath(m_projectInfo.RootFolderAbsolute, path);

            // The custom build rules (if there are any) are per configuration. So
            // we loop through the configurations for this file...
            IVCCollection configurations = Utils.call(() => (file.FileConfigurations as IVCCollection));
            int numConfigurations = Utils.call(() => (configurations.Count));
            for (int i = 1; i <= numConfigurations; ++i)
            {
                // We check if there is a rule for each configuration...
                VCFileConfiguration configuration = Utils.call(() => (configurations.Item(i) as VCFileConfiguration));
                parseCustomBuildRule_Configuration(configuration, relativePath); ;
            }
        }

        /// <summary>
        /// Parses one configuration for a custom build rule.
        /// </summary>
        private void parseCustomBuildRule_Configuration(VCFileConfiguration configuration, string relativePath)
        {
            // We find the custom build rule for this configuration (if there is one)...
            VCCustomBuildRule rule = Utils.call(() => (configuration.Tool as VCCustomBuildRule));
            if (rule == null)
            {
                return;
            }

            // We will store info about this rule in a CustomBuildRuleInfo_CPP object...
            CustomBuildRuleInfo_CPP ruleInfo = new CustomBuildRuleInfo_CPP();
            ruleInfo.RelativePathToFile = relativePath;

            // There is a custom build rule, so we parse it...
            ruleInfo.RuleName = Utils.call(() => (rule.Name));
            string commandLine = Utils.call(() => (rule.CommandLine));
            string expandedCommandLine = Utils.call(() => (configuration.Evaluate(commandLine)));

            // We find the collection of output files generated by the rule...
            string outputs = Utils.call(() => (rule.Outputs));
            string expandedOutputs = Utils.call(() => (configuration.Evaluate(outputs)));

            // The command-line may contain references to custom properties for
            // the rule. These will be in square brackets,like [property]. We find
            // these and replace the property markers in the command-line...
            Dictionary<string, string> properties = getCustomBuildRuleProperties(rule);
            foreach (KeyValuePair<string, string> property in properties)
            {
                // We replace values in the command line, and in the output files...
                string tokenToReplace = "[" + property.Key + "]";
                expandedCommandLine = expandedCommandLine.Replace(tokenToReplace, property.Value);
                expandedOutputs = expandedOutputs.Replace(tokenToReplace, property.Value);
            }

            // We split the command-line to find the executable and the parameters...
            List<string> splitCommandLine = Utils.split(expandedCommandLine, ' ');

            // The executable...
            if (splitCommandLine.Count >= 1)
            {
                string ruleExecutable = splitCommandLine[0];
                string relativePathToExecutable = Utils.makeRelativePath(m_projectInfo.RootFolderAbsolute, ruleExecutable);
                ruleInfo.RelativePathToExecutable = relativePathToExecutable;
            }

            // The parameters...
            for (int j = 1; j < splitCommandLine.Count; ++j)
            {
                string parameter = splitCommandLine[j];

                // Many parameters will be path names. We cnvert these
                // to be relative to the project root (and in Linux format).
                // Non-path parameters should be unaffected...
                parameter = Utils.makeRelativePath(m_projectInfo.RootFolderAbsolute, parameter);
                ruleInfo.addParameter(parameter);
            }

            // We store the info in the configuration...
            m_projectInfo.addCustomBuildRuleInfo(ruleInfo, configuration.Name);
        }


        /// <summary>
        /// Finds the collection of properties associated with the custom build
        /// rule passed in. These are passed back in a map of:
        /// PropertyName => PropertyValueString
        /// </summary>
        private Dictionary<string, string> getCustomBuildRuleProperties(VCCustomBuildRule rule)
        {
            Dictionary<string, string> results = new Dictionary<string, string>();

            // We loop through the collection of properties for the rule...
            IVCCollection properties = Utils.call(() => (rule.Properties));
            int numProperties = Utils.call(() => (properties.Count));
            for (int i = 1; i <= numProperties; ++i)
            {
                VCRuntimeProperty property = Utils.call(() => (properties.Item(i) as VCRuntimeProperty));

                // We find the property's name...
                string name = property.Name;

                // We have to use this rather bizarre method to get the property's value.
                // (It seems that the property is a dynamically created COM object.)
                Type ruleType = rule.GetType();
                object valueAsObject = Utils.call(() => (ruleType.InvokeMember(name, BindingFlags.GetProperty, null, rule, null)));
                string valueAsString = valueAsObject.ToString();

                results.Add(name, valueAsString);
            }

            return results;
        }

        #endregion

        #region Private data

        // Holds the parsed project data...
        private ProjectInfo_CPP m_projectInfo = new ProjectInfo_CPP();

        // The root folder of the solution that this project is part of...
        private string m_solutionRootFolder = "";

        // The Visual Studio project object...
        private VCProject m_vcProject = null;

        #endregion
    }
}
