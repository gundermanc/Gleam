# Untitled 'Christian-Gunderman' project
# (C) 2019 Christian Gunderman

# Ensure we're in the right environment.
if (-not (Get-Command "devenv.exe" -ErrorAction SilentlyContinue))
{
    Write-Host -ForegroundColor Yellow "Must be run from within Visual Studio Developer Command"
}

# Bootstrap repo.
.\Init.ps1

# Build and set VC path.
.\Build.ps1

# Launch Visual Studio.
devenv .