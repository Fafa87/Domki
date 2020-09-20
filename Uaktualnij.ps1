# UPEWNIJ SIE ZE UAKTUALNIJ JEST W BINIE!!!

Remove-Item -Force -recurse * -exclude Uaktualnij.ps1

Invoke-WebRequest "https://ci.appveyor.com/api/projects/fafa87/domki/artifacts/Domki.zip?branch=master&job=Image%3A%20Visual%20Studio%202017" -OutFile Domki_nowe.zip

expand-archive -path Domki_nowe.zip -destinationpath .