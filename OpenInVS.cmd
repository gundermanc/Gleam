@echo off

:: Untitled 'Christian-Gunderman' project
:: (C) 2019 Christian Gunderman

:: This file exists to bypass Powershell Execution Policy.

powershell.exe -ExecutionPolicy Bypass -Command ".\OpenInVS.ps1" %*
