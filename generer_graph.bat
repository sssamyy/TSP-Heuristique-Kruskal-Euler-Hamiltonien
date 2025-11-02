@echo off
echo Génération des images depuis les fichiers DOT...

REM Récupère le chemin absolu du dossier contenant ce script
set SCRIPT_DIR=%~dp0

REM Définit le chemin vers dot.exe à partir du dossier du script
set DOT_PATH=%SCRIPT_DIR%graphviz\bin\dot.exe

IF NOT EXIST "%DOT_PATH%" (
    echo Erreur : dot.exe introuvable à "%DOT_PATH%"
    pause
    exit /b
)

for %%f in (*.dot) do (
    echo Traitement de %%f ...
    "%DOT_PATH%" -Tpng "%%f" -o "%%~nf.png"
)

echo Terminé !
pause
﻿