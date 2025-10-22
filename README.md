# OBSBOT Linux GUI

Ubuntu22.04環境でOBSBOTカメラの操作を行うGUIアプリケーションです。

## 機能

- OBSBOT USBカメラの自動検出と接続
- 8方向のパンチルト操作（↑↓←→↖↗↙↘）
- センター位置へのリセット機能
- 速度調整スライダー（0.1倍〜2.0倍）
- デバイス情報の表示（デバイス名、バージョン、シリアル番号）

## 必要な環境

- Ubuntu 22.04 LTS
- GTK+ 3.0
- CMake 3.16以上
- C++11対応コンパイラ
- OBSBOT SDK v2.1.0_7 （https://www.obsbot.jp/jp/sdk の`お問い合わせ`から必要事項を入力して各自ダウンロード）

## 依存パッケージのインストール

```bash
sudo apt update
sudo apt install build-essential cmake pkg-config libgtk-3-dev
```

## ビルド方法

### 方法1: ビルドスクリプトを使用

```bash
chmod +x build.sh
./build.sh
```

### 方法2: 手動ビルド

```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
make install
```

## 実行方法

### 方法1: 実行スクリプトを使用（推奨）

```bash
./run.sh
```

### 方法2: 手動で実行

```bash
cd build/rundir
LD_LIBRARY_PATH=. ./obsbot_pantilt_gui
```

## 使い方

1. OBSBOTカメラをUSBで接続します
2. アプリケーションが自動的にデバイスを検出します
3. 方向ボタンを押してパンチルト操作を行います
   - ボタンを押している間、カメラが移動します
   - ボタンを離すと停止します
4. 「Center」ボタンでカメラをセンター位置に戻します
5. 速度スライダーで移動速度を調整できます

## 操作方法

- **方向ボタン**: カメラの向きを変更（8方向）
- **Center**: カメラをセンター位置（0°, 0°）に移動
- **Speed Multiplier**: 移動速度の倍率を調整（0.1〜2.0倍）
- **Disconnect**: デバイスとの接続を解除

## トラブルシューティング

### デバイスが検出されない場合

1. USBケーブルが正しく接続されているか確認
2. デバイスの電源が入っているか確認
3. USB権限の問題がある場合は、udevルールを設定する必要があります

### ビルドエラーが発生する場合

1. GTK3がインストールされているか確認:

   ```bash
   pkg-config --modversion gtk+-3.0
   ```

2. SDKパスが正しいか確認:
   - CMakeLists.txtのSDK_INCLUDE_PATHとSDK_LINK_PATHを確認

3. アーキテクチャが対応しているか確認（x86_64またはarm64）

## ライセンス

OBSBOT SDKのライセンスに従います。

## 技術情報

- **GUI Framework**: GTK+ 3.0
- **Build System**: CMake
- **Language**: C++11
- **SDK**: OBSBOT libdev v2.1.0_7

## 開発情報

### ディレクトリ構造

```
OBSBOT_Linux_GUI/
├── CMakeLists.txt                  # CMake設定ファイル
├── main.cpp                        # メインプログラム
├── build.sh                        # ビルドスクリプト
├── README.md                       # このファイル
├── build/                          # ビルド出力（生成される）
｜     └── rundir/                     # 実行可能ファイルとライブラリ
├── OBSBOT_SDK_機能一覧.md          # OBSBOT SDKの機能一覧（日本語）
├── OBSBOT_SDK_Features_List.md     # OBSBOT SDKの機能一覧（英語）
└── libdev_v2.1.0_7/                # OBSBOT SDK（各自ダウンロード）
       └── linux(beta)/
              ├── arm64-release/    # arm64環境 Linux共有ライブラリ
              └── x86_64-release/   # x86_64環境 Linux共有ライブラリ
```

### 主要なAPI

- `Devices::get().setDevChangedCallback()`: デバイス接続/切断イベントの監視
- `Device::aiSetGimbalSpeedCtrlR()`: ジンバル速度制御
- `Device::aiSetGimbalMotorAngleR()`: ジンバル角度制御（絶対位置）

APIで実装可能なその他の機能は、[OBSBOT_SDK_機能一覧.md](./OBSBOT_SDK_機能一覧.md) を参照してください。

## 注意事項

- （Ver.1.0.0現在）このアプリケーションはOBSBOT Tiny SEでのみ動作確認しています  
  他のOBSBOT製品では動作しない可能性があります  
- カメラの可動範囲を超える操作はできません  
- USB接続が必須です（ネットワーク接続は未対応）  

## バージョン情報

- 1.0.0 (2025.10.23)  
  パンチルト操作を実装  
  Ubuntu22.04/x86_64-releaseでビルド、Tiny SEで動作確認済み
