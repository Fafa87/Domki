Invoke-WebRequest "https://ci.appveyor.com/api/projects/fafa87/domki/artifacts/_Instalka%2Fbin%2FRelease%2FDomcraft.msi?branch=master&job=Image%3A%20Visual%20Studio%202017" -OutFile Domcraft.msi

msiexec /i Domcraft.msi REINSTALL=ALL REINSTALLMODE=vemus /passive

Remove-Item Domcraft.msi