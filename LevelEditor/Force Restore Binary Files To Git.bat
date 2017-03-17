@ECHO OFF

FOR /F "tokens=*" %%f IN ('dir LevelEditor\bin\Debug /b /a:-d') DO (
    echo      Restoring %%f
    git update-index --no-assume-unchanged "LevelEditor\bin\Debug\%%f"
)


FOR /F "tokens=*" %%f IN ('dir LevelEditor\bin\Debug\Assets /b /a:-d') DO (
    echo      Restoring %%f
    git update-index --no-assume-unchanged "LevelEditor\bin\Debug\Assets\%%f"
)

FOR /F "tokens=*" %%f IN ('dir LevelEditor\bin\Release /b /a:-d') DO (
    echo      Restoring %%f
    git update-index --no-assume-unchanged "LevelEditor\bin\Release\%%f"
)

FOR /F "tokens=*" %%f IN ('dir LevelEditor\bin\Release\Assets /b /a:-d') DO (
    echo      Restoring %%f
    git update-index --no-assume-unchanged "LevelEditor\bin\Release\Assets\%%f"
)

FOR /F "tokens=*" %%f IN ('dir LevelEditor\obj\Debug /b /a:-d') DO (
    echo      Restoring %%f
    git update-index --no-assume-unchanged "LevelEditor\obj\Debug\%%f"
)

FOR /F "tokens=*" %%f IN ('dir LevelEditor\obj\Release /b /a:-d') DO (
    echo      Restoring %%f
    git update-index --no-assume-unchanged "LevelEditor\obj\Release\%%f"
)

FOR /F "tokens=*" %%f IN ('dir LevelEditor\x64\Debug /b /a:-d') DO (
    echo      Restoring %%f
    git update-index --no-assume-unchanged "LevelEditor\x64\Debug\%%f"
)

pause