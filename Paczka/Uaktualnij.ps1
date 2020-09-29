# UPEWNIJ SIE ZE UAKTUALNIJ JEST W BINIE!!!

if ((Test-Path Domki.exe) -and (Test-Path MultiSerwer.exe)) {
Remove-Item -Force -recurse * -exclude Uaktualnij.ps1

Invoke-WebRequest "https://ci.appveyor.com/api/projects/fafa87/domki/artifacts/Domki.zip?branch=master&job=Image%3A%20Visual%20Studio%202017" -OutFile Domki_nowe.zip

expand-archive -path Domki_nowe.zip -destinationpath .
}
else 
{

Invoke-WebRequest "https://ci.appveyor.com/api/projects/fafa87/domki/artifacts/Domki.zip?branch=master&job=Image%3A%20Visual%20Studio%202017" -OutFile Domki.zip

expand-archive -path Domki.zip -destinationpath Domki

$ScriptName = Split-Path $PSCommandPath -Leaf
Move-Item $ScriptName Domki/Uaktualnij.ps1

}
