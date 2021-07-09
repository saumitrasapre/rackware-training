Write-Host "The following is the information about logical drives installed on this computer"
Write-Host "Computer: $env:COMPUTERNAME"

$dsize = @{label = "DiskSize (GB)"; expression = {"{0:N}" -f ($_.Size/1GB) -as [float]}}
$fspace = @{label = "FreeSpace (GB)"; expression = {"{0:N}" -f ($_.FreeSpace/1GB) -as [float]}}
$ospace = @{label = "OccSpace (GB)"; expression = {"{0:N}" -f ($_.Size/1GB - $_.FreeSpace/1GB) -as [float]}}
$percfree =  @{label = "% Free"; expression = {"{0:N}" -f ($_.FreeSpace/$_.Size * 100) -as [float]}}

$physize = @{label = "Size (GB)"; expression = {"{0:N}" -f ($_.Size/1GB) -as [float]}}

#region Logical Drive info using WMI
# Get-WmiObject -Namespace root/CIMV2 -Class Win32_LogicalDisk |`
#     Where-Object {$_.DriveType -ne 5} |`
#     Sort-Object -Descending -Property FreeSpace | `
#     Select-Object Name, VolumeName, FileSystem, Description, $dsize, $fspace, $percfree| `
#     Format-Table -AutoSize
#endregion

#region Logical Drive Info using Cim
 Get-CimInstance -Namespace root/CIMV2 -ClassName Win32_LogicalDisk |`
    Where-Object {$_.DriveType -ne 5} |`
    Sort-Object -Descending -Property FreeSpace | `
    Select-Object Name, VolumeName, FileSystem, Description, $dsize,$fspace, $ospace,$percfree | `
    Format-Table -AutoSize   
#endregion  

Write-Host "The following is the information about logical drives installed on this computer (Sorted according to occupied space)"
Write-Host "Computer: $env:COMPUTERNAME"
#region Logical Drive Info using Cim
Get-CimInstance -Namespace root/CIMV2 -ClassName Win32_LogicalDisk |`
Where-Object {$_.DriveType -ne 5} |`
Sort-Object -Descending -Property @{Expression = {$_.Size - $_.FreeSpace}}| `
Select-Object Name, VolumeName, FileSystem, Description, $dsize,$fspace,$ospace, $percfree | `
Format-Table -AutoSize   
#endregion  

Write-Host "The following is the information about physical drives installed on this computer"
Write-Host "Computer: $env:COMPUTERNAME"

# #region Physical Drive Info using WMI
# Get-WmiObject -Namespace root/CIMV2 -Class Win32_DiskDrive |`
#     Sort-Object -Descending -Property Size |`
#     Select-Object DeviceID, Caption, Model, $physize | `
#     Format-Table -AutoSize 

# #endregion

#region Physical Drive Info using Cim
Get-CimInstance -Namespace root/CIMV2 -ClassName Win32_DiskDrive |`
    Sort-Object -Descending -Property Size |`
    Select-Object DeviceID, Caption, Model, $physize | `
    Format-Table -AutoSize 

#endregion

