﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MakeItSoLib;
using Microsoft.VisualStudio.VCProjectEngine;
using VSLangProj80;
using VSLangProj;
using System.IO;
using FileInfo = MakeItSoLib.FileInfo;

namespace SolutionParser_VS2008
{
    /// <summary>
    /// Parses a C# project.
    /// </summary><remarks>
    /// We extract information from a DTE Project and a VSProject2 object, and fill 
    /// in a ProjectInfo structure.
    /// </remarks>
    internal class ProjectParser_CSharp
    {
        #region Public methods and properties

        /// <summary>
        /// Constructor
        /// </summary>
        public ProjectParser_CSharp(VSProject2 vsProject, string solutionRootFolder)
        {
            try
            {
                m_vsProject = vsProject;
                m_solutionRootFolder = solutionRootFolder;
                m_dteProject = Utils.call(() => vsProject.Project);

                // We get the project name...
                m_projectInfo.Name = Utils.call(() => (m_dteProject.Name));
                
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
        public ProjectInfo_CSharp Project 
        {
            get { return m_projectInfo; }
        }

        #endregion

        #region Private functions

        /// <summary>
        /// Parses the project to find the data we need to build the makefile.
        /// </summary>
        private void parseProject()
        {
            // Note: We need to parse the project properties and configurations 
            //       first, as the later stages need to know things like the 
            //       project root folder, and which configurations there are...
            parseProjectProperties();
            parseConfigurations();

            // We parse the rest of the project...
            parseFiles();
            parseReferences();
        }

        /// <summary>
        /// We find the collection of references needed by this project.
        /// </summary>
        private void parseReferences()
        {
            // We loop through the collection of references, adding
            // them to the project. (There is a second pass later to 
            // resolve references to other projects that may not have
            // been parsed yet.)
            References references = Utils.call(() => (m_vsProject.References));
            int numReferences = Utils.call(() => (references.Count));
            for (int i = 1; i <= numReferences; ++i)
            {
                Reference reference = Utils.call(() => references.Item(i));
                string fullPath = Utils.call(() => (reference.Path));
                bool copyLocal = Utils.call(() => (reference.CopyLocal));
                m_projectInfo.addReference(fullPath, copyLocal);
            }
        }

        /// <summary>
        /// Parses the project-level properties, such as the project type
        /// and so on.
        /// </summary>
        private void parseProjectProperties()
        {
            // We convert the DTE properties to a map...
            EnvDTE.Properties dteProperties = Utils.call(() => (m_dteProject.Properties));
            Dictionary<string, object> projectProperties = getProperties(dteProperties);

            // The project type (exe, library etc)...
            prjOutputType outputType = (prjOutputType)getIntProperty(projectProperties, "OutputType");
            switch(outputType)
            {
                case prjOutputType.prjOutputTypeExe:
                    m_projectInfo.ProjectType = ProjectInfo.ProjectTypeEnum.CSHARP_EXECUTABLE;
                    break;

                case prjOutputType.prjOutputTypeLibrary:
                    m_projectInfo.ProjectType = ProjectInfo.ProjectTypeEnum.CSHARP_LIBRARY;
                    break;

                case prjOutputType.prjOutputTypeWinExe:
                    m_projectInfo.ProjectType = ProjectInfo.ProjectTypeEnum.CSHARP_WINFORMS_EXECUTABLE;
                    break;
            }

            // The output file name, e.g. TextLib.dll...
            m_projectInfo.OutputFileName = getStringProperty(projectProperties, "OutputFileName");

            // The project folder, absolute and relative to the solution...
            m_projectInfo.RootFolderAbsolute = getStringProperty(projectProperties, "FullPath");
            m_projectInfo.RootFolderRelative = Utils.makeRelativePath(m_solutionRootFolder, m_projectInfo.RootFolderAbsolute);
        }

        /// <summary>
        /// We find the collection of configurations (Release, Debug etc)
        /// and parse each one.
        /// </summary>
        private void parseConfigurations()
        {
            EnvDTE.ConfigurationManager configurationManager = Utils.call(() => (m_dteProject.ConfigurationManager));
            int numConfigurations = Utils.call(() => (configurationManager.Count));
            for (int i = 1; i <= numConfigurations; ++i)
            {
                EnvDTE.Configuration dteConfiguration = Utils.call(() => (configurationManager.Item(i, "") as EnvDTE.Configuration));
                parseConfiguration(dteConfiguration);
            }
        }

        /// <summary>
        /// Parses on configuration.
        /// </summary>
        private void parseConfiguration(EnvDTE.Configuration dteConfiguration)
        {
            // We create a new configuration-info object and fill it in...
            ProjectConfigurationInfo_CSharp configurationInfo = new ProjectConfigurationInfo_CSharp();
            configurationInfo.ParentProjectInfo = m_projectInfo;
            configurationInfo.Name = Utils.call(() => dteConfiguration.ConfigurationName);

            // We parse the configuration's properties, and set configuration
            // seetings from them...
            EnvDTE.Properties dteProperties = Utils.call(() => (dteConfiguration.Properties));
            Dictionary<string, object> properties = getProperties(dteProperties);

            // Whether to optimize...
            configurationInfo.Optimize = getBoolProperty(properties, "Optimize");

            // The output path and intermediate path...
            configurationInfo.OutputFolder = getStringProperty(properties, "OutputPath");
            configurationInfo.IntermediateFolder = getStringProperty(properties, "IntermediatePath");

            // Whether to treat warnings as errors...
            configurationInfo.ThreatWarningsAsErrors = getBoolProperty(properties, "TreatWarningsAsErrors");

            // Defined constants (DEBUG, TRACE etc)...
            string definedConstants = getStringProperty(properties, "DefineConstants");
            foreach (string definedConstant in Utils.split(definedConstants, ';'))
            {
                configurationInfo.addDefinedConstant(definedConstant);
            }

            // Whether to add debug symbols to the output...
            configurationInfo.Debug = getBoolProperty(properties, "DebugSymbols");

            // Comma separated list of warnings to ignore...
            string warningsToIgnore = getStringProperty(properties, "NoWarn");
            foreach (string warningToIgnore in Utils.split(warningsToIgnore, ','))
            {
                configurationInfo.addWarningToIgnore(warningToIgnore);
            }

            // DebugInfo, e.g. "full"...
            configurationInfo.DebugInfo = getStringProperty(properties, "DebugInfo");

            // File alignment...
            configurationInfo.FileAlignment = getIntProperty(properties, "FileAlignment");

            // Warning level...
            configurationInfo.WarningLevel = getIntProperty(properties, "WarningLevel");

            // We add the configuration-info to the project-info...
            m_projectInfo.addConfigurationInfo(configurationInfo);
        }

        /// <summary>
        /// Gets a bool property from the collection of properties passed in.
        /// Returns false if the property is not in the collection.
        /// </summary>
        private bool getBoolProperty(Dictionary<string, object> properties, string name)
        {
            return (properties.ContainsKey(name) == true) ? (bool)properties[name] : false;
        }

        /// <summary>
        /// Gets a string property from the collection of properties passed in.
        /// Returns "" if the property is not in the collection.
        /// </summary>
        private string getStringProperty(Dictionary<string, object> properties, string name)
        {
            return (properties.ContainsKey(name) == true) ? (string)properties[name] : "";
        }

        /// <summary>
        /// Gets an int property from the collection of properties passed in.
        /// Returns 0 if the property is not in the collection.
        /// </summary>
        private int getIntProperty(Dictionary<string, object> properties, string name)
        {
            return (properties.ContainsKey(name) == true) ? Convert.ToInt32(properties[name]) : 0;
        }

        /// <summary>
        /// Converts the collection of properties passed in into a map 
        /// of string -> object.
        /// </summary>
        private Dictionary<string, object> getProperties(EnvDTE.Properties dteProperties)
        {
            Dictionary<string, object> results = new Dictionary<string, object>();

            // Some properties do not seem to have valid values. These are
            // the main ones we have trouble with, so we will ignore them and
            // not try to retrieve their values...
            HashSet<string> slowProperties = new HashSet<string> { "WebServer", "ServerExtensionsVersion", "OfflineURL", "WebServerVersion", "WebAccessMethod", "ActiveFileSharePath", "AspnetVersion", "FileSharePath" };

            // We loop through the properties...
            int numProperties = Utils.call(() => (dteProperties.Count));
            for (int i = 1; i <= numProperties; ++i)
            {
                try
                {
                    EnvDTE.Property dteProperty = Utils.call(() => (dteProperties.Item(i)));
                    string propertyName = Utils.call(() => (dteProperty.Name));
                    if (slowProperties.Contains(propertyName) == true)
                    {
                        // This is one of the properties to ignore...
                        continue;
                    }

                    object propertyValue = Utils.call(() => (dteProperty.Value));
                    results[propertyName] = propertyValue;
                }
                catch (Exception)
                {
                    // Some of the properties don't seem to have valid values.
                    // I'm not really sure why this is. But we silently catch 
                    // the exception, as they don't seem to be the properties
                    // we need anyway.
                }
            }

            return results;
        }

        /// <summary>
        /// Finds the collection of .cs files in the project.
        /// </summary>
        private void parseFiles()
        {
            // We find the collection of files...
            HashSet<FileInfo> fileInfos = new HashSet<FileInfo>();
            EnvDTE.ProjectItems projectItems = Utils.call(() => (m_dteProject.ProjectItems));
            findFiles(projectItems, fileInfos);

            // We decide what to do with the files we've found...
            foreach (FileInfo fileInfo in fileInfos)
            {
                // We add the .cs files to the project info...
                if (fileInfo.Extension == ".cs")
                {
                    m_projectInfo.addFile(fileInfo.RelativePath);
                }

                // We check if the file should be copied to the output folder...
                if (fileInfo.CopyToOutputFolder == true)
                {
                    m_projectInfo.addFileToCopyToOutputFolder(fileInfo);
                }
            }
        }

        /// <summary>
        /// Find all .cs files in the project, including sub-folders.
        /// </summary>
        private void findFiles(EnvDTE.ProjectItems projectItems, HashSet<FileInfo> files)
        {
            // We look through the items...
            int numProjectItems = Utils.call(() => (projectItems.Count));
            for (int i = 1; i <= numProjectItems; ++i)
            {
                EnvDTE.ProjectItem projectItem = Utils.call(() => (projectItems.Item(i)));

                // We get the full-path...
                EnvDTE.Properties dteProperties = Utils.call(() => (projectItem.Properties));
                Dictionary<string, object> properties = getProperties(dteProperties);
                string fullPath = getStringProperty(properties, "FullPath");
                int copyToOutputFolder = getIntProperty(properties, "CopyToOutputDirectory");

                // We check if it is a file (it could be a folder instead)...
                if (File.Exists(fullPath) == true)
                {
                    // We add it to the list of files...
                    FileInfo fileInfo = new FileInfo();
                    fileInfo.AbsolutePath = fullPath;
                    fileInfo.RelativePath = Utils.makeRelativePath(m_projectInfo.RootFolderAbsolute, fullPath);
                    fileInfo.CopyToOutputFolder = (copyToOutputFolder != 0);
                    files.Add(fileInfo);
                }

                // We see if the item itself has sub-items...
                EnvDTE.ProjectItems subItems = Utils.call(() => (projectItem.ProjectItems));
                if (subItems != null)
                {
                    findFiles(subItems, files);
                }

                // We see if this item has a sub-project...
                EnvDTE.Project subProject = Utils.call(() => (projectItem.SubProject));
                if (subProject != null)
                {
                    EnvDTE.ProjectItems subProjectItems = Utils.call(() => (subProject.ProjectItems));
                    findFiles(subProjectItems, files);
                }
            }
        }

        #endregion

        #region Private data

        // Holds the parsed project data...
        private ProjectInfo_CSharp m_projectInfo = new ProjectInfo_CSharp();

        // The root folder of the solution that this project is part of...
        private string m_solutionRootFolder = "";

        // The Visual Studio project objects. We need two of these: the
        // EnvDte project which as overall project info, and the VSProject2
        // which has C#-specific info...
        private EnvDTE.Project m_dteProject = null;
        private VSProject2 m_vsProject = null;

        #endregion
    }
}
