@set path=E:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE;%path%

@cls
copy "公共文件\GlobalDef.h" "(游戏组件)\Include"

devenv "共享组件\公共服务\ComService.vcproj" /build release
devenv "共享组件\界面控件\SkinControls.vcproj" /build release
devenv "共享组件\网络组件\SocketModule.vcproj" /build release




devenv "客户端组件\客户端共享\ClientShare.vcproj" /build release
devenv "客户端组件\信道模块\ChannelModule.vcproj" /build release
devenv "客户端组件\关系管理\Companion.vcproj" /build release
devenv "客户端组件\头像组件\UserFace.vcproj" /build release
devenv "客户端组件\下载组件\DownLoad.vcproj" /build release
devenv "客户端组件\游戏等级\GameRank.vcproj" /build release

devenv "客户端组件\游戏广场(红网)\GamePlaza.vcproj" /build release
devenv "客户端组件\游戏框架(红网)\GameFrame.vcproj" /build release

