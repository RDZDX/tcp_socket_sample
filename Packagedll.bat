"C:\Program Files\MRE_SDK\tools\DllPackage.exe" "D:\mre app\tcp_socket_sample\tcp_socket_sample.vcproj"
if %errorlevel% == 0 (
 echo postbuild OK.
  copy tcp_socket_sample.vpp ..\..\..\MoDIS_VC9\WIN32FS\DRIVE_E\tcp_socket_sample.vpp /y
exit 0
)else (
echo postbuild error
  echo error code: %errorlevel%
  exit 1
)

