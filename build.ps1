# ==============================

# FileSend Windows Build Script

# ==============================

$CC = "gcc"

# Paths

$SRC_DIR = "src"
$DIST_DIR = "dist/windows/filesend"
$TARGET = "$DIST_DIR/filesend.exe"

# Create output directory

New-Item -ItemType Directory -Force -Path $DIST_DIR | Out-Null

Write-Host "Building FileSend (Windows)..."

# Build command (explicit arguments to avoid PowerShell issues)

$cmd = @(
"$SRC_DIR/*.c",
"-Iinclude",
"-o", $TARGET,
"-lws2_32",
"-lwinpthread",
"-lncursesw",
"-mconsole"
)

# Execute build

& $CC $cmd

if ($LASTEXITCODE -ne 0) {
Write-Host "Build failed!"
exit 1
}

Write-Host "Build successful!"
Write-Host "Output: $TARGET"

# ==============================

# Copy required DLLs

# ==============================

Write-Host "Copying required DLLs..."

$MSYS_BIN = "C:\msys64\ucrt64\bin"

Copy-Item "$MSYS_BIN\libncursesw6.dll" -Destination $DIST_DIR -Force
Copy-Item "$MSYS_BIN\libwinpthread-1.dll" -Destination $DIST_DIR -Force

Write-Host "All dependencies copied."
Write-Host "Done. Ready for installer."
