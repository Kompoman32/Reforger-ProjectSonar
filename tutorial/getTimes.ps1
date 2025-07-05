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

    $time = $shellfolder.GetDetailsOf($shellfile, 27);
    
    $seconds = [int]$time.Split(":")[2]
    $minutes = [int]$time.Split(":")[1]
    $hours = [int]$time.split(":")[0]

    $total = $hours * 60 * 60 + $minutes * 60 + $seconds

    Write-Host $total;
}

return;
