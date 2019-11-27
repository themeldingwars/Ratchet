$NET_CORE_DL_URL = "https://download.visualstudio.microsoft.com/download/pr/ae0efd9e-e55d-4148-9d95-dd3e5fd8e6d2/5297cf422f6b4818077a6f459acde520/dotnet-runtime-2.2.5-win-x86.zip"
$NET_CORE_DIR = "Staging\Ratchet\.Net Core\"
$NET_CORE_ZIP = "$($NET_CORE_DIR)NetCore.zip"

# Download .Net Core
Invoke-WebRequest $NET_CORE_DL_URL -O $NET_CORE_ZIP
Expand-Archive -Path $NET_CORE_ZIP -DestinationPath "$($NET_CORE_DIR)Temp" -Force
Copy-Item "$($NET_CORE_DIR)Temp\shared\Microsoft.NETCore.App\2.2.5\*" -Destination $NET_CORE_DIR -Recurse
Remove-Item "$($NET_CORE_DIR)Temp" -Recurse
Remove-Item "$($NET_CORE_ZIP)"
