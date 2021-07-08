#region Starting with Hello world
Write-Output "Hello " + $env:USERNAME

Write-Host "Hello $env:USERNAME"

Write-Output "$env:COMPUTERNAME"
#endregion

#region Difference between write-output and write-host

function Receive-Output {
   process { Write-Host $_ -ForegroundColor Green}
    
}

Write-Host "You are looking " -NoNewline
Write-Host "AWESOME" -ForegroundColor Red -BackgroundColor Yellow -NoNewline
Write-Host "today"
#endregion

#region Variables, single and double quotes
$name = "Moe"
Write-Output "Hello $name"
Write-Output 'Hello $name'
Write-Output "Hello `t`t`t World"
#endregion

#region Getting User Input
$uname = Read-Host "Enter your username"
$pass  = Read-Host "Enter your password" -AsSecureString
Write-Output "Hello $uname"
[Runtime.InteropServices.Marshal]::PtrToStringAuto([Runtime.InteropServices.Marshal]::SecureStringToBSTR($pass))
#endregion
