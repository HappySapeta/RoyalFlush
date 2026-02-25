winget install -e --id Microsoft.VisualStudio.BuildTools

"C:\Program Files (x86)\Microsoft Visual Studio\Installer\setup.exe" modify ^
--installPath "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools" ^
--add Microsoft.VisualStudio.Workload.ManagedDesktopBuildTools ^
--add Microsoft.VisualStudio.Workload.VCTools ^
--add Microsoft.VisualStudio.Component.Windows11SDK.26100 ^
--add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
--add Microsoft.VisualStudio.Component.VC.14.44.17.14.x86.x64 ^
--add Microsoft.NetCore.Component.SDK ^
--add Microsoft.Net.ComponentGroup.DevelopmentPrerequisites ^
--add Microsoft.Net.ComponentGroup.4.8.DeveloperTools ^