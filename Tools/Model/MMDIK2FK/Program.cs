using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MikuMikuDance.Model;
using MikuMikuDance.Motion;
using MikuMikuDance.Motion.Motion2;
using MikuMikuDance.Model.Ver1;
using MMDIKBakerLibrary;

namespace MMDIK2FK
{
    class Program
    {
        static void PrintUsage()
        {
            Console.WriteLine("MMDIK2FK Usage");
            Console.WriteLine("This tool converts IK to FK in vmd file.");
            Console.WriteLine("MMDIK2FK,exe [pmd] [vmd] [dst]");
        }

        static int Main(string[] args)
        {
            if (args.Length != 3)
            {
                PrintUsage();
                return 1;
            }

            var pmd = args[0];
            var vmd = args[1];
            var dst = args[2];

            try
            {
                var model = (MMDModel1)ModelManager.Read(
                    pmd,
                    MikuMikuDance.Model.CoordinateType.LeftHandedCoordinate);

                var motion = (MMDMotion2)MotionManager.Read(
                    vmd,
                    MikuMikuDance.Motion.CoordinateType.LeftHandedCoordinate);

                var dstMotion = IKBaker.bake(motion, model);
                MotionManager.Write(dst, dstMotion);

                return 0;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Failed : [{0}]", ex.Message);
                PrintUsage();
            }

            return 1;
        }
    }
}
