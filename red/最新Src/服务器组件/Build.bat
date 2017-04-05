@set path=E:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE;%path%

@cls

devenv "登陆服务器\LogonServer.vcproj" /build release
devenv "服务装载器\ServiceLoader.vcproj" /build release
devenv "列表服务\ListService.vcproj" /build release
devenv "内核组件\ServerKernel.vcproj" /build release
devenv "游戏服务\GameService.vcproj" /build release
devenv "中心服务器\CenterServer.vcproj" /build release

