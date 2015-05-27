@echo off
if defined VS90COMNTOOLS (
call "%VS90COMNTOOLS%\vsvars32.bat")
buildwin 90 rebuild static_md both x64 nosamples
