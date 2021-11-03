[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
Invoke-WebRequest http://github.com/Fafa87/Domki/releases/latest/download/Domcraft.msi -OutFile Domcraft.msi

msiexec /i Domcraft.msi /passive

Remove-Item Domcraft.msi  # spróbuj chociaż go usunąć

#Start-Process "msiexec.exe" -ArgumentList '/i Domcraft.msi /passive' -Wait &&  Remove-Item Domcraft.msi -erroraction 'silentlycontinue'

#while ((Test-Path "Domcraft.msi")) 
#{ 
#    Start-Sleep 5
#    Remove-Item Domcraft.msi -erroraction 'silentlycontinue'
#}


