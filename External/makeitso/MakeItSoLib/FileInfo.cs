﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace MakeItSoLib
{
    /// <summary>
    /// Holds information about one file in a project.
    /// </summary>
    public class FileInfo
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        public FileInfo()
        {
            IsFromAProjectOutputFolder = false;
        }

        /// <summary>
        /// Returns a new copy of this FileInfo.
        /// </summary>
        public FileInfo clone()
        {
            FileInfo result = new FileInfo();
            result.AbsolutePath = AbsolutePath;
            result.CopyToOutputFolder = CopyToOutputFolder;
            result.RelativePath = RelativePath;
            result.IsFromAProjectOutputFolder = IsFromAProjectOutputFolder;
            return result;
        }

        /// <summary>
        /// Gets or sets the absolute path.
        /// </summary>
        public string AbsolutePath { get; set; }

        /// <summary>
        /// Gets or sets the path to the file, relative to the root folder of
        /// the project that it belongs to.
        /// </summary>
        public string RelativePath { get; set; }

        /// <summary>
        /// Gets or sets whether the file should be copied to the output folder.
        /// </summary>
        public bool CopyToOutputFolder { get; set; }

        /// <summary>
        /// Gets or sets whether this file is from a project's output
        /// folder (e.g. bin/Debug/file.ext). 
        /// </summary><remarks>
        /// If it is, we will need to change the folder name when adding 
        /// it to the makefile.
        /// </remarks>
        public bool IsFromAProjectOutputFolder { get; set; }

        /// <summary>
        /// Gets the file extension, including the '.'
        /// </summary>
        public string Extension
        {
            get { return Path.GetExtension(AbsolutePath); }
        }
    }
}
