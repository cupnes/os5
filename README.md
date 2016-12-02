# OS5

## 概要

* フルスクラッチの自作OS
* x86のQEMU(qemu-system-i386)で動作
* 最新リリース等、詳しくは「[自作OS(OS5)のまとめ](http://funlinux.org/os5/)」

## 使い方

* gcc
* make
* qemu

しか使っていないので、

以下のDebianパッケージがインストールされていればビルドと動作確認を行えます。

* build-essential
* qemu

以下でビルドからqemuでの実行まで行います。

`make run`

## ドキュメントについて

docディレクトリ以下のドキュメントはemacsのorg-modeで記述しています。

テキストファイルなのでそのままでも読めますが、`make doc`で整形されたテキストファイルへエクスポートできます。
