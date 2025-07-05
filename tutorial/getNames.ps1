[CmdletBinding()]
param(
    [Parameter(Mandatory = $false)]
    [string]$Path = "."
)

$files = Get-ChildItem "$($path)*.wav"

if ($files.Length -eq 0){
    Write-Host "No wav found." -ForegroundColor Red
    return
}


foreach ($f in $files){
    $shell = New-Object -COMObject Shell.Application
    $folder = Split-Path $f.fullname
    $file = Split-Path $f.fullname -Leaf
    $shellfolder = $shell.Namespace($folder)
    $shellfile = $shellfolder.ParseName($file)

    $name = $shellfolder.GetDetailsOf($shellfile, 0);
    $name = $name.Replace('.wav', '');
    Write-Host `"$name`" ;
}

return;
