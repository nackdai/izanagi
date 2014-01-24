﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace MakeItSoLib
{
    /// <summary>
    /// An base class for solution-parsers.
    /// </summary><remarks>
    /// To parse a new type of solution (e.g. VS2008, VS2010 etc) you should:
    /// - Create an assembly for the parser.
    /// - Create a class in it that derives from SolutionParserBase, 
    ///   and implement the abstract methods.
    /// - Register the class with the main MakeItSo project
    /// </remarks>
    public abstract class SolutionParserBase
    {
        #region Abstract methods

        /// <summary>
        /// Parses the solution in the file passed in.
        /// </summary>
        public abstract void parse(string solutionFilename);

        #endregion

        #region Public methods and properties

        /// <summary>
        /// Returns the parsed solution.
        /// </summary>
        public SolutionInfo ParsedSolution
        {
            get { return m_parsedSolution; }
        }

        /// <summary>
        /// Updates the solution with any changes specified in the MakeItSo.config file.
        /// </summary>
        public void updateSolutionFromConfig()
        {
            // We check each configuration in each project...
            foreach (ProjectInfo projectInfo in m_parsedSolution.getProjectInfos())
            {
                if (projectInfo is ProjectInfo_CPP)
                {
                    updateProjectFromConfig_CPP(projectInfo as ProjectInfo_CPP);
                }
            }
        }

        #endregion

        #region Private functions

        /// <summary>
        /// Updates the C++ project passed in with data from the MakeItSo.config file.
        /// </summary>
        private void updateProjectFromConfig_CPP(ProjectInfo_CPP project)
        {
            foreach (ProjectConfigurationInfo_CPP configuration in project.getConfigurationInfos())
            {
                updateLibraries(configuration);
                updateLibraryPaths(configuration);
                updateIncludePaths(configuration);
                updatePreprocessorDefinitions(configuration);
                updateCompilerFlags(configuration);
            }
        }

        /// <summary>
        /// Updates compiler flags from the config settings.
        /// </summary>
        private void updateCompilerFlags(ProjectConfigurationInfo_CPP configuration)
        {
            MakeItSoConfig_Project projectSettings = MakeItSoConfig.Instance.getProjectConfig(configuration.ParentProjectInfo.Name);

            // We check if any definitions should be removed...
            List<string> flags = new List<string>(configuration.getCompilerFlags());
            foreach (string flag in flags)
            {
                if (projectSettings.compilerFlagShouldBeRemoved(flag) == true)
                {
                    configuration.removeCompilerFlag(flag);
                }
            }

            // We add any new definitions...
            List<string> flagsToAdd = projectSettings.getConfiguration(configuration.Name).getCompilerFlagsToAdd();
            foreach (string flag in flagsToAdd)
            {
                configuration.addCompilerFlag(flag);
            }
        }

        /// <summary>
        /// Updates preprocessor definitions from config settings.
        /// </summary>
        private void updatePreprocessorDefinitions(ProjectConfigurationInfo_CPP configuration)
        {
            MakeItSoConfig_Project projectSettings = MakeItSoConfig.Instance.getProjectConfig(configuration.ParentProjectInfo.Name);

            // By default we replace WIN32 with GCC_BUILD...
            configuration.removePreprocessorDefinition("WIN32");
            configuration.addPreprocessorDefinition("GCC_BUILD");

            // We check if any definitions should be removed...
            List<string> definitions = new List<string>(configuration.getPreprocessorDefinitions());
            foreach (string definition in definitions)
            {
                if(projectSettings.preprocessorDefinitionShouldBeRemoved(definition) == true)
                {
                    configuration.removePreprocessorDefinition(definition);
                }
            }

            // We add any new definitions...
            List<string> definitionsToAdd = projectSettings.getConfiguration(configuration.Name).getPreprocessorDefinitionsToAdd();
            foreach (string definition in definitionsToAdd)
            {
                configuration.addPreprocessorDefinition(definition);
            }
        }

        /// <summary>
        /// Updates library paths from config settings.
        /// </summary>
        private void updateLibraryPaths(ProjectConfigurationInfo_CPP configuration)
        {
            MakeItSoConfig_Project projectSettings = MakeItSoConfig.Instance.getProjectConfig(configuration.ParentProjectInfo.Name);

            ProjectInfo projectInfo = configuration.ParentProjectInfo;
            string projectRootFolder = projectInfo.RootFolderAbsolute;

            // We check if any library paths should be removed...
            List<string> libraryPaths = new List<string>(configuration.getLibraryPaths());
            foreach (string libraryPath in libraryPaths)
            {
                // We remove the library (and re-add it if we need to, but
                // with the name changed)...
                configuration.removeLibraryPath(libraryPath);

                // We find the full path, and add it if we are not
                // configured to remove it...
                string fullPath = Path.Combine(projectRootFolder, libraryPath);
                if (projectSettings.libraryPathShouldBeRemoved(fullPath) == false)
                {
                    string prefix = MakeItSoConfig.Instance.getProjectConfig(projectInfo.Name).CPPFolderPrefix;
                    string gccPath = Utils.addPrefixToFolderPath(libraryPath, prefix);
                    configuration.addLibraryPath(gccPath);
                }
            }

            // We add any new paths...
            List<string> pathsToAdd = projectSettings.getConfiguration(configuration.Name).getLibraryPathsToAdd();
            foreach (string pathToAdd in pathsToAdd)
            {
                string relativePath = Utils.makeRelativePath(projectRootFolder, pathToAdd);
                configuration.addLibraryPath(relativePath);
            }
        }

        /// <summary>
        /// Updates include paths from config settings.
        /// </summary>
        private void updateIncludePaths(ProjectConfigurationInfo_CPP configuration)
        {
            MakeItSoConfig_Project projectSettings = MakeItSoConfig.Instance.getProjectConfig(configuration.ParentProjectInfo.Name);

            string projectRootFolder = configuration.ParentProjectInfo.RootFolderAbsolute;

            // We check if any include paths should be removed...
            List<string> includePaths = new List<string>(configuration.getIncludePaths());
            foreach (string includePath in includePaths)
            {
                string fullPath = Path.Combine(projectRootFolder, includePath);
                if (projectSettings.includePathShouldBeRemoved(fullPath) == true)
                {
                    configuration.removeIncludePath(includePath);
                }
            }

            // We add any new paths...
            List<string> pathsToAdd = projectSettings.getConfiguration(configuration.Name).getIncludePathsToAdd();
            foreach (string pathToAdd in pathsToAdd)
            {
                string relativePath = Utils.makeRelativePath(projectRootFolder, pathToAdd);
                configuration.addIncludePath(relativePath);
            }
        }

        /// <summary>
        /// Updates libraries from config settings.
        /// </summary>
        private void updateLibraries(ProjectConfigurationInfo_CPP configuration)
        {
            MakeItSoConfig_Project projectSettings = MakeItSoConfig.Instance.getProjectConfig(configuration.ParentProjectInfo.Name);

            // We check if any of the libraries in the configuration should be removed...
            HashSet<string> libraries = new HashSet<string>(configuration.getLibraryRawNames());
            foreach (string library in libraries)
            {
                if (projectSettings.libraryShouldBeRemoved(library) == true)
                {
                    configuration.removeLibraryRawName(library);
                }
            }

            // We add any that need adding...
            List<string> librariesToAdd = projectSettings.getConfiguration(configuration.Name).getLibrariesToAdd();
            foreach (string library in librariesToAdd)
            {
                string rawName = Utils.convertLinuxLibraryNameToRawName(library);
                configuration.addLibraryRawName(rawName);
            }
        }

        #endregion

        #region Protected data

        // Holds the parsed solution data, including the 
        // collection of projects in it...
        protected SolutionInfo m_parsedSolution = new SolutionInfo();

        #endregion
    }
}
