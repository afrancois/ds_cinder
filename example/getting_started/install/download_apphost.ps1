Add-Type -AssemblyName System.IO.Compression.FileSystem

$DsnodeVer = (Invoke-WebRequest -Uri 'http://update.downstreamdev.com/ds/dsapphost/version.txt').Content;
Write-Host $DsnodeVer;


$DsnodeUrl = "http://update.downstreamdev.com/ds/dsapphost/" + $DsnodeVer;
$ThisCommand = $MyInvocation.MyCommand.Path;
$ThisDir = (Get-Item $ThisCommand ).Directory.FullName;
$TempFile = $ThisDir + "/dsnoder.zip";
$DsnodeDir = $ThisDir + "/DSAppHost/";
$OutputDir = $ThisDir;

if (Test-Path $DsnodeDir -PathType Container){
    Write-Host Removing old dsnode dir...;
    Remove-Item $DsnodeDir -Recurse;
}

Write-Host Downloading dsnode zip...;
Invoke-WebRequest -Uri $DsnodeUrl -OutFile $TempFile;

Write-Host Extracting dsnode zip...
[System.IO.Compression.ZipFile]::ExtractToDirectory($TempFile, $OutputDir);

Write-Host Removing zip file...;
Remove-Item $TempFile;

if (Test-Path $DsnodeDir -PathType Container){
    Write-Host Success!;
} else {
    Write-Host Uh oh, something went wrong!;
}
