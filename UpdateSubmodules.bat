@echo off

git submodule init
git submodule update --recursive

echo Submodules were updated successfully.
pause