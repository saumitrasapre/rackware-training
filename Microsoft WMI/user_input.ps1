#region Testing Parameters with WMI

param ([Parameter(Mandatory=$true)][string] $computername)
Get-WmiObject -Class win32_computersystem -ComputerName $computername | Format-List numberofprocessors,totalphysicalmemory

#endregion

#region Testing Parameters with CIM

Param ([string] $computername = $env:COMPUTERNAME)
Get-CimInstance -ClassName win32_computersystem -ComputerName $computername | Format-List numberofprocessors,totalphysicalmemory

#endregion