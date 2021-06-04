Invoke-WebRequest "https://github.com/Fafa87/Domki/releases/latest/download/Domcraft.msi" -OutFile Domcraft.msi

msiexec /i Domcraft.msi /passive

Remove-Item Domcraft.msi