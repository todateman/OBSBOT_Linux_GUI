# OBSBOT SDK 制御機能一覧

OBSBOT SDK v2.1.0_7 で利用可能な制御機能の完全なリストです。

---

## 目次

1. [パンチルト制御](#1-パンチルト制御)
2. [ジンバル制御](#2-ジンバル制御)
3. [ズーム制御](#3-ズーム制御)
4. [フォーカス制御](#4-フォーカス制御)
5. [AIトラッキングモード](#5-aiトラッキングモード)
6. [カメラ設定](#6-カメラ設定)
7. [FOV(視野角)設定](#7-fov視野角設定)
8. [プリセット位置](#8-プリセット位置)
9. [HDR/WDR設定](#9-hdrwdr設定)
10. [メディアモード](#10-メディアモード)
11. [デバイスステータスクエリ](#11-デバイスステータスクエリ)
12. [ジェスチャー制御](#12-ジェスチャー制御)
13. [音声制御](#13-音声制御)
14. [その他の設定機能](#14-その他の設定機能)

---

## 1. パンチルト制御

### 相対移動制御

#### `cameraSetPanTiltRelative(double pan_speed, double tilt_speed)`
- **目的**: ビデオプレビューのパンチルト軸の相対移動速度を設定
- **パラメータ**:
  - `pan_speed`: パン速度、範囲: -1.0〜1.0
  - `tilt_speed`: チルト速度、範囲: -1.0〜1.0
- **対応製品**: Meet, Meet4K, Meet2, MeetSE

### 絶対位置制御

#### `cameraSetPanTiltAbsolute(double pan_deg, double tilt_deg)`
- **目的**: ビデオプレビューのパンチルト軸の絶対移動位置を設定
- **パラメータ**:
  - `pan_deg`: パン角度、範囲: -1.0〜1.0
  - `tilt_deg`: チルト角度、範囲: -1.0〜1.0
- **対応製品**: Meet, Meet4K, Meet2, MeetSE

---

## 2. ジンバル制御

### ジンバル速度制御

#### `aiSetGimbalSpeedCtrlR(double pitch, double pan, double roll)`
- **目的**: ジンバルの回転速度を設定、0に設定してジンバルを停止可能
- **パラメータ**:
  - `pitch`: ピッチ速度、範囲: -90〜90
  - `pan`: パン速度、範囲: -180〜180
  - `roll`: ロール速度、範囲: -180〜180（現在未使用）
- **注意**: AIスマートトラッキングが有効な場合、ジンバルは常にAIによって制御される
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

#### `gimbalSpeedCtrlR(double pitch, double pan, double roll)`
- **目的**: ジンバルの速度を設定（AIトラッキング無効化必須）
- **パラメータ**:
  - `pitch`: ピッチ速度、範囲: -90〜90
  - `pan`: パン速度、範囲: -180〜180
  - `roll`: ロール速度、範囲: -90〜90（現在未使用、0に設定）
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

#### `aiSetGimbalStop()`
- **目的**: ジンバルを停止
- **対応製品**: Tiny2シリーズ, Tail Air

### ジンバル位置制御

#### `aiSetGimbalMotorAngleR(float pitch, float yaw, float roll)`
- **目的**: ジンバルを指定したモーター角度に移動
- **パラメータ**:
  - `pitch`: 目標ピッチ角度、範囲: -90〜90
  - `yaw`: 目標ヨー角度、範囲: -180〜180
  - `roll`: 目標ロール角度、範囲: -180〜180（現在未使用、0に設定）
- **対応製品**: Tiny2シリーズ, Tail Air

#### `gimbalSetSpeedPositionR(float roll, float pitch, float yaw, float s_roll, float s_pitch, float s_yaw)`
- **目的**: ジンバルの目標位置と目標位置への移動参照速度を設定
- **パラメータ**:
  - `roll`: ロールの目標位置（未使用）
  - `pitch`: ピッチの目標位置、範囲: -90〜90
  - `yaw`: ヨーの目標位置、範囲: -120〜120
  - `s_roll`: ロール参照速度、範囲: -90〜90
  - `s_pitch`: ピッチ参照速度、範囲: -90〜90
  - `s_yaw`: ヨー参照速度、範囲: -90〜90
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

### ジンバルリセット

#### `gimbalRstPosR()` / `aiResetGimbalR()`
- **目的**: ジンバルをゼロ位置（センター）にリセット
- **注意**: AIスマートトラッキングが有効な場合、ジンバルは常にAIによって制御される
  - Tiny/Tiny4K: `aiSetTargetSelectR(false)` でAIを無効化
  - Tiny2/Tail Air: `cameraSetAiModeU(Device::AiWorkModeNone)` でAIを無効化
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

### ジンバル状態取得

#### `aiGetGimbalStateR(AiGimbalStateInfo *gim_info, ...)`
- **目的**: 現在のジンバル状態情報を同期/非同期で取得
- **戻り値**: AiGimbalStateInfo構造体（ロール/ピッチ/ヨーのオイラー角、モーター角、角速度）
- **対応製品**: Tiny2シリーズ, Tail Air

#### `gimbalGetAttitudeInfoR(float xyz[3], ...)`
- **目的**: ジンバルのモーター角度を同期/非同期で取得
- **パラメータ**: xyz配列（ロール、ピッチ、パン角度を度単位で受信）
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

### ジンバルパラメータ

#### `aiSetGimbalSpeedR(float speed)`
- **目的**: ジンバルの速度を設定
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

#### `aiSetGimbalParaR(DevGimbalParaType type, ...)`
- **目的**: ジンバル機能パラメータを設定
- **パラメータタイプ**:
  - `PresetSpeed`: プリセット速度（Tail Airサポート）
  - `PanGainAdaptive`: パンゲイン適応
  - `PanGainValue`: パンゲインカスタム値
  - `PitchGainAdaptive`: ピッチゲイン適応
  - `PitchGainValue`: ピッチゲインカスタム値
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

#### `aiGetGimbalParaR(DevGimbalParaType type, ...)`
- **目的**: ジンバル機能パラメータを取得
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

---

## 3. ズーム制御

### 基本ズーム機能

#### `cameraSetZoomAbsoluteR(float zoom)`
- **目的**: カメラのズームレベルを設定
- **パラメータ**: zoom (1.0〜2.0の正規化値)
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Meet, Meet4K, Tail Air

#### `cameraGetZoomAbsoluteR(float &zoom)`
- **目的**: 正規化されたズーム値を取得
- **戻り値**: 1.0〜2.0のズーム値
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Meet, Meet4K, Tail Air

#### `cameraGetRangeZoomAbsoluteR(UvcParamRange &range)`
- **目的**: ズームパラメータの範囲（最小値、最大値、ステップ値）を取得
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Meet, Meet4K, Tail Air

### 高度なズーム機能

#### `cameraSetZoomWithSpeedAbsoluteR(uint32_t zoom_ratio, uint32_t zoom_speed)`
- **目的**: ズーム比率とズーム速度を設定
- **パラメータ**:
  - `zoom_ratio`: 目標ズーム比率（実際の値の100倍）
  - `zoom_speed`: 0=デフォルト速度、1-10=速度値（大きいほど速い）、255=最大速度
- **対応製品**: Tiny2, Tail Air

#### `cameraSetZoomWithSpeedRelativeR(uint32_t zoom_step, uint32_t zoom_speed, bool step_mode, bool in_out)`
- **目的**: 相対モードでズーム速度、方向、モードを設定
- **パラメータ**:
  - `zoom_step`: ズームステップ（step_mode=trueの場合のみ有効）
  - `zoom_speed`: ズーム速度
  - `step_mode`: true=ステップモード、false=連続モード
  - `in_out`: true=ズームイン、false=ズームアウト
- **対応製品**: Tail Air

#### `cameraSetZoomStopR()`
- **目的**: ズームを即座に停止
- **対応製品**: Tail Air

### AIオートズーム

#### `aiSetAiAutoZoomR(bool enabled)`
- **目的**: AIのオートズーム機能を有効/無効化
- **対応製品**: Tiny2シリーズ, Tail Air

#### `aiSetTargetZoomTypeR(DevTargetZoomType zoom_type, DevCustomizedZoomType customized_zoom_type)`
- **目的**: 現在のターゲットのズームタイプを設定
- **ズームタイプ**: Normal, FullBody, HalfBody, CloseUp, Customized, GroupHeadless, GroupLowerBody, Adaptive
- **対応製品**: Tiny2シリーズ, Tail Air

---

## 4. フォーカス制御

### オートフォーカスモード

#### `cameraSetAutoFocusModeR(DevAutoFocusType focus_type)`
- **目的**: オートフォーカスタイプを設定
- **フォーカスタイプ**:
  - `AutoSelect`: 自動選択
  - `AFC`: 連続オートフォーカス
  - `AFS`: シングルオートフォーカス
  - `MF`: マニュアルフォーカス
- **対応製品**: Tail Air

#### `cameraGetAutoFocusModeR(DevAutoFocusType &focus_type)`
- **目的**: 現在のオートフォーカスタイプを取得
- **対応製品**: Tail Air

### マニュアルフォーカス

#### `cameraSetFocusPosR(int32_t focus_pos)`
- **目的**: フォーカスモーター位置を設定
- **パラメータ**: focus_pos (0〜100)
- **対応製品**: Tail Air

#### `cameraGetFocusPosR(int32_t &focus_pos)`
- **目的**: 現在のフォーカスモーター位置を取得
- **対応製品**: Tail Air

### フェイスフォーカス

#### `cameraSetFaceFocusR(bool enable)`
- **目的**: フェイスフォーカス機能を有効/無効化
- **対応製品**: 全製品

#### `cameraSetFaceAER(int32_t face_ae)`
- **目的**: 顔認識オート露出を有効/無効化
- **パラメータ**: 0=無効、1=有効
- **対応製品**: 全製品

### 連続オートフォーカス設定

#### `cameraSetAFCTrackModeR(DevAFCType afc_type)`
- **目的**: 連続オートフォーカスのトラッキングタイプを設定
- **AFCタイプ**: Center（中央）, Face（顔）, AiObject（AIオブジェクト）, Foreground（前景）
- **対応製品**: Tail Air

#### `cameraGetAFCTrackModeR(DevAFCType &afc_type)`
- **目的**: 現在の連続オートフォーカスタイプを取得
- **対応製品**: Tail Air

---

## 5. AIトラッキングモード

### トラッキングモード設定

#### `aiSetAiTrackModeEnabledR(AiTrackModeType ai_mode, bool enabled)`
- **目的**: AIトラッキングモードを有効/無効化
- **モード**:
  - `AiTrackNormal`: 通常モード
  - `AiTrackHumanNormal`: 人物通常モード
  - `AiTrackHumanFullBody`: 人物全身モード
  - `AiTrackHumanHalfBody`: 人物半身モード
  - `AiTrackHumanCloseUp`: 人物クローズアップモード
  - `AiTrackHumanAutoView`: 人物オートビューモード
  - `AiTrackAnimalNormal`: 動物通常モード
  - `AiTrackAnimalCloseUp`: 動物クローズアップモード
  - `AiTrackAnimalAutoView`: 動物オートビューモード
  - `AiTrackGroup`: グループモード
- **対応製品**: Tail Air

#### `aiSetTrackingModeR(AiVerticalTrackType mode)`
- **目的**: AIスマートトラッキングモードを設定
- **モード**:
  - `AiVTrackStandard`: 標準モード
  - `AiVTrackHeadroom`: ヘッドルームモード
  - `AiVTrackMotion`: モーションモード
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

### トラッキング速度

#### `aiSetTrackSpeedTypeR(AiTrackSpeedType track_type)`
- **目的**: AIのトラッキング速度を設定
- **速度タイプ**: Lazy（遅延）, Slow（遅い）, Standard（標準）, Fast（速い）, Crazy（超速）, Auto（自動）
- **対応製品**: Tail Air

### ターゲット選択

#### `aiSelectTargetInBoxR(int32_t left, int32_t top, int32_t width, int32_t height, ...)`
- **目的**: 指定したボックス内のトラッキングターゲットを選択
- **パラメータ**: ボックスの座標とサイズ（正規化された0〜10000の値）
- **対応製品**: Tiny2シリーズ, Tail Air

#### `aiSelectBiggestTargetR(AiTargetType target_type)`
- **目的**: 画像内の最大ターゲットを自動選択してトラッキング
- **ターゲットタイプ**: Auto（自動）, Person（人）, Cat（猫）, Dog（犬）, Horse（馬）, Animal（動物）
- **対応製品**: Tiny2シリーズ, Tail Air

#### `aiSelectCenterTargetR(AiTargetType target_type)`
- **目的**: 画像中央のターゲットを選択してトラッキング
- **対応製品**: Tiny2シリーズ, Tail Air

#### `aiCancelSelectTargetR()`
- **目的**: 選択したトラッキングターゲットをキャンセル
- **対応製品**: Tiny2シリーズ, Tail Air

### ゾーントラッキング

#### `aiSetZoneTrackStateR(bool enabled)`
- **目的**: ゾーントラッキング機能を有効/無効化
- **対応製品**: Tiny2シリーズ, Tail Air

#### `aiSetLimitedZoneTrackEnabledR(bool enabled)`
- **目的**: 制限ゾーントラッキング領域機能を有効/無効化
- **対応製品**: Tiny2シリーズ

#### `aiSetLimitedZoneTrackYawMinR(float deg)` / `aiSetLimitedZoneTrackYawMaxR(float deg)`
- **目的**: 制限ゾーントラッキングのヨー角度範囲を設定
- **パラメータ**: 角度（度）
- **対応製品**: Tiny2シリーズ

#### `aiSetLimitedZoneTrackPitchMinR(float deg)` / `aiSetLimitedZoneTrackPitchMaxR(float deg)`
- **目的**: 制限ゾーントラッキングのピッチ角度範囲を設定
- **パラメータ**: 角度（度）
- **対応製品**: Tiny2シリーズ

#### `aiSetLimitedZoneTrackAutoSelectR(bool enabled)`
- **目的**: 領域制限トラッキングの自動ターゲット選択を有効/無効化
- **対応製品**: Tiny2シリーズ

### ハンドトラッキング

#### `aiSetHandTrackTypeR(DevHandTrackType hand_type)`
- **目的**: ハンドトラッキングタイプを設定（左手/右手）
- **対応製品**: Tiny2シリーズ, TinySE

---

## 6. カメラ設定

### 露出制御

#### `cameraSetExposureModeR(int32_t exposure_mode)`
- **目的**: シャッターモードを設定
- **モード**: Unknown, Manual（マニュアル）, AllAuto（全自動）, AperturePriority（絞り優先）, ShutterPriority（シャッター優先）
- **対応製品**: Tail Air

#### `cameraSetExposureAbsolute(int32_t shutter_time, bool auto_enabled)`
- **目的**: シャッター値とオート露出モードを設定
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraGetExposureAbsolute(int32_t &shutter_time, bool &auto_enabled)`
- **目的**: シャッター時間とオート露出モードを取得
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraSetAELockR(bool enabled)`
- **目的**: AE（自動露出）ロックを有効/無効化
- **対応製品**: Tail Air

### EVBIAS設定（露出補正）

#### `cameraSetPAEEvBiasR(int32_t ev_bias)`
- **目的**: Pギア（絞り優先）でのオート露出のEVBIASを設定
- **範囲**: -3.0 〜 +3.0（DevAEEvBiasType列挙型）
- **対応製品**: Tail Air

#### `cameraSetSAEEvBiasR(int32_t ev_bias)`
- **目的**: Sギア（シャッター優先）でのオート露出のEVBIASを設定
- **対応製品**: Tail Air

#### `cameraSetAAEEvBiasR(int32_t ev_bias)`
- **目的**: Aギア（自動）でのオート露出のEVBIASを設定
- **対応製品**: Tail Air

### ISO設定

#### `cameraSetISOLimitR(uint32_t min_iso, uint32_t max_iso)`
- **目的**: 最大・最小ISO値を設定
- **対応製品**: Tail Air

#### `cameraGetISOLimitR(uint32_t &min_iso, uint32_t &max_iso)`
- **目的**: 現在の最大・最小ISO値を取得
- **対応製品**: Tail Air

#### `cameraSetMAEIsoR(int32_t iso)` / `cameraGetMAEIsoR(int32_t &iso)`
- **目的**: Mギア（マニュアル）でのオートISO露出を設定/取得
- **対応製品**: Tail Air

### ホワイトバランス

#### `cameraSetWhiteBalanceR(DevWhiteBalanceType wb_type, int32_t param)`
- **目的**: ホワイトバランスパラメータを設定
- **WBタイプ**:
  - Auto（自動）, Daylight（太陽光）, Fluorescent（蛍光灯）, Tungsten（タングステン）
  - Flash（フラッシュ）, Fine（晴天）, Cloudy（曇天）, Manual（マニュアル）など24種類
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraGetWhiteBalanceR(DevWhiteBalanceType &wb_type, int32_t &param)`
- **目的**: 現在のホワイトバランスパラメータを取得
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraGetWhiteBalanceListR(std::vector<int32_t> &wb_list, int32_t &wb_min, int32_t &wb_max)`
- **目的**: サポートされているホワイトバランスリストと範囲を取得
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

### アンチフリッカー

#### `cameraSetAntiFlickR(int32_t freq)`
- **目的**: アンチフリッカーモードを設定
- **周波数**: Off（オフ）, 50Hz, 60Hz, Auto（自動）
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraGetAntiFlickR(int32_t &freq)`
- **目的**: 現在のアンチフリッカーモードを取得
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

### 画像調整

#### `cameraSetImageBrightnessR(int32_t brightness)`
- **目的**: 画像の明るさを設定
- **範囲**: 0〜100
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraSetImageContrastR(int32_t contrast)`
- **目的**: 画像のコントラストを設定
- **範囲**: 0〜100
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraSetImageSaturationR(int32_t saturation)`
- **目的**: 画像の彩度を設定
- **範囲**: 0〜100
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraSetImageSharpR(int32_t sharp)`
- **目的**: 画像のシャープネスを設定
- **範囲**: 0〜100
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraSetImageHueR(int32_t hue)`
- **目的**: 画像の色相を設定
- **範囲**: 0〜100
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

### 画像スタイル

#### `cameraSetImageStyleR(DevImageStyle style)`
- **目的**: 画像スタイルを設定
- **スタイル**: Standard（標準）, Text（テキスト）, LandScape（風景）, Portrait（ポートレート）, NightScape（夜景）, Film（フィルム）, Customer（カスタム）
- **対応製品**: Tail Air

---

## 7. FOV(視野角)設定

#### `cameraSetFovU(FovType fov_type)`
- **目的**: カメラのFOV（視野角）を設定
- **FOVタイプ**:
  - `FovType86`: 86度（広角）
  - `FovType78`: 78度（中角）
  - `FovType65`: 65度（狭角）
- **対応製品**: Tinyシリーズ, Meetシリーズ

---

## 8. プリセット位置

### ジンバルプリセット

#### `aiGetGimbalPresetListR(DevDataArray *ids, ...)`
- **目的**: 既存のプリセット位置のIDリストを取得
- **対応製品**: Tiny2シリーズ, Tail Air

#### `aiGetGimbalPresetInfoWithIdR(PresetPosInfo *preset_info, int32_t id, ...)`
- **目的**: 指定IDのプリセット位置情報を取得
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

#### `aiAddGimbalPresetR(PresetPosInfo *preset_info)`
- **目的**: プリセット位置を追加（既存の場合は更新）
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air

#### `aiDelGimbalPresetR(int32_t id)`
- **目的**: 指定IDのプリセット位置を削除
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air

#### `aiTrgGimbalPresetR(int pos_id)`
- **目的**: 指定IDのプリセット位置にジンバルを移動
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

### 起動時プリセット

#### `aiSetGimbalBootPosR(const PresetPosInfo &preset_info, bool presets_flag)`
- **目的**: 起動時の初期位置とズーム比率を設定
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

#### `aiGetGimbalBootPosR(PresetPosInfo *preset_info, ...)`
- **目的**: ジンバル起動情報を取得
- **対応製品**: Tiny, Tiny4K, Tiny2, Tail Air

#### `aiRstGimbalBootPosR()`
- **目的**: 起動時の初期位置をデフォルト値にリセット
- **対応製品**: Tiny2シリーズ, Tail Air

#### `aiTrgGimbalBootPosR(bool reset_mode)`
- **目的**: ジンバルを起動時の初期位置に移動
- **対応製品**: Tiny2シリーズ, Tail Air

---

## 9. HDR/WDR設定

#### `cameraSetWdrR(int32_t wdr_mode)`
- **目的**: WDR/HDR機能を設定
- **モード**:
  - `DevWdrModeNone`: HDR無効
  - `DevWdrModeDol2TO1`: HDR有効
- **注意**: モード切り替えは時間がかかるため、3秒以上の間隔を推奨
- **対応製品**: Tiny4K, Tiny2シリーズ, Meetシリーズ, Tail Air

#### `cameraGetWdrR(int32_t &wdr_mode)`
- **目的**: 現在のWDR状態を取得
- **対応製品**: Tail Air

#### `cameraGetWdrListR(std::vector<int32_t> &wdr_list)`
- **目的**: サポートされているWDR状態のリストを取得
- **対応製品**: Tail Air

---

## 10. メディアモード

### メディアモード設定（Meet製品専用）

#### `cameraSetMediaModeU(MediaMode mode)`
- **目的**: メディアモードを設定
- **モード**:
  - `MediaModeNormal`: 通常モード
  - `MediaModeBackground`: バーチャル背景モード
  - `MediaModeAutoFrame`: オートフレーミングモード
- **注意**: モード切り替えは時間がかかるため、3秒以上の間隔を推奨
- **対応製品**: Meet, Meet4K

### バーチャル背景（Meet製品専用）

#### `cameraSetBgModeU(MediaBgMode mode)`
- **目的**: バーチャル背景モードを設定
- **モード**:
  - `MediaBgModeDisable`: なし
  - `MediaBgModeColor`: グリーンモード
  - `MediaBgModeReplace`: 置換モード
  - `MediaBgModeBlur`: ぼかしモード
- **対応製品**: Meet, Meet4K

#### `cameraSetBgColorU(MediaBgModeColorType bg_color)`
- **目的**: グリーンモードでの背景色を設定
- **色**: Blue（青）, Green（緑）, Red（赤）, Black（黒）, White（白）
- **対応製品**: Meet, Meet4K

#### `cameraSetBgBlurLevelU(int32_t level)`
- **目的**: 背景のぼかしレベルを設定
- **対応製品**: Meet, Meet4K

### オートフレーミング（Meet製品専用）

#### `cameraSetAutoFramingModeU(AutoFramingType group_single, AutoFramingType close_upper)`
- **目的**: オートフレームモードを設定
- **モード**: AutoFrmGroup（グループ）, AutoFrmSingle（シングル）, AutoFrmCloseUp（クローズアップ）, AutoFrmUpperBody（上半身）
- **対応製品**: Meetシリーズ

### レコーディング（Tail Air専用）

#### `cameraSetVideoRecordR(uint32_t operation, uint32_t param)`
- **目的**: ビデオレコーディングを開始/停止
- **操作**: 0=停止、1=開始
- **対応製品**: Tail Air

#### `cameraSetRecordResolutionR(DevVideoResType res_type)`
- **目的**: レコーディング用のビデオ解像度を設定
- **対応製品**: Tail Air

#### `cameraSetRecordSplitSizeR(DevVideoSplitSizeType split_type)`
- **目的**: レコーディング時のビデオファイル分割サイズを設定
- **対応製品**: Tail Air

---

## 11. デバイスステータスクエリ

### ステータス取得

#### `cameraStatus()`
- **目的**: カメラステータスを取得（2-3秒のラグがある可能性あり）
- **戻り値**: CameraStatus構造体

#### `cameraGetCameraStatusU(CameraStatus &camera_status)`
- **目的**: カメラステータス情報を取得
- **対応製品**: 全製品

#### `aiGetAiStatusR(AiStatus *ai_status, ...)`
- **目的**: 現在のAIステータスを同期/非同期で取得
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ

### ステータスコールバック

#### `setDevStatusCallbackFunc(DevStatusCallback callback, void *param)`
- **目的**: DevStatusCallbackを登録（2-3秒ごとにデバイスステータスをプッシュ）

#### `enableDevStatusCallback(bool enabled)`
- **目的**: DevStatusCallbackを有効/無効化

### 動作状態

#### `cameraSetDevRunStatusR(DevStatus type)`
- **目的**: デバイスの動作状態を設定
- **状態**:
  - `DevStatusRun`: 通常モード
  - `DevStatusSleep`: スリープモード
  - `DevStatusPrivacy`: プライバシーモード（ストリーム停止）
- **対応製品**: 全製品

---

## 12. ジェスチャー制御

### ジェスチャー機能

#### `aiSetGestureCtrlR(bool flag)`
- **目的**: ジェスチャーコントロール全体をオン/オフ
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Tail Air

#### `aiSetGestureCtrlIndividualR(int32_t gesture, bool flag)`
- **目的**: 指定したジェスチャー機能をオン/オフ
- **ジェスチャータイプ**:
  - `0`: Gesture target（ジェスチャーターゲット）
  - `1`: Gesture zoom（ジェスチャーズーム）
  - `2`: Gesture dynamic zoom（ジェスチャーダイナミックズーム）- Tiny2シリーズ, Tail Air
  - `3`: Gesture dynamic zoom direction - Tiny2シリーズ, Tail Air
  - `4`: Gesture record（ジェスチャーレコード）- Tail Air
- **対応製品**: Tiny2シリーズ, Tail Air

### ジェスチャーパラメータ

#### `aiSetGestureParaR(DevGestureParaType type, ...)`
- **目的**: ジェスチャー機能パラメータを設定
- **パラメータタイプ**:
  - `Gesture`: ジェスチャー機能全体
  - `TargetSelection`: ターゲット選択ジェスチャー
  - `Zoom`: ズームジェスチャー
  - `DynamicZoom`: ダイナミックズームジェスチャー
  - `Record`: レコードジェスチャー
  - `Snapshot`: スナップショットジェスチャー
  - `Rolling`: ローリングジェスチャー
  - `Mirror`: ミラー
  - `ZoomFactor`: ハンドポーズズーム比率

#### `aiGetGestureParaR(DevGestureParaType type, ...)`
- **目的**: ジェスチャー機能パラメータを取得

### ジェスチャートラッキングパラメータ

#### `aiSetGestureTrackParaR(DevGestureTrackParaType type, ...)`
- **目的**: ジェスチャートラッキング機能パラメータを設定
- **パラメータタイプ**:
  - `PanMin` / `PanMax`: パン制限
  - `PitchMin` / `PitchMax`: ピッチ制限
  - `HandType`: 左手/右手トラッキング
  - `TrackSpeed`: トラッキング速度
  - `PanEnabled` / `PitchEnabled`: パン/ピッチ有効化
  - `RestSeconds`: 境界到達後、初期位置に戻るまでの秒数

#### `aiGetGestureTrackParaR(DevGestureTrackParaType type, ...)`
- **目的**: ジェスチャートラッキング機能パラメータを取得

---

## 13. 音声制御

### 音声コマンド（Tiny2シリーズ専用）

#### `cameraSetVoiceCtrlU(AudioCtrlCmdType cmd, int32_t state)`
- **目的**: 指定した音声制御コマンドを有効/無効化、言語選択、ズーム係数設定
- **コマンドタイプ**:
  - `AudioCtrlHiTiny`: Hi Tiny（起動コマンド）
  - `AudioCtrlSleepTiny`: Sleep Tiny（スリープコマンド）
  - `AudioCtrlTrack`: Track Me（トラッキング開始）
  - `AudioCtrlUnlock`: Unlock Me（トラッキング解除）
  - `AudioCtrlZoomIn`: Zoom In Close（ズームイン）
  - `AudioCtrlZoomOut`: Zoom Out Further（ズームアウト）
  - `AudioCtrlPreset`: Preset Position 1/2/3（プリセット位置）
  - `AudioCtrlZoomFactor`: ズーム係数設定用（0〜100）
  - `AudioCtrlLanguage`: 音声制御言語設定
- **言語**: Chinese (ZhCN), English (EnUS)
- **対応製品**: Tiny2シリーズ

---

## 14. その他の設定機能

### 画像フリップ・ミラー

#### `cameraSetImageFlipHorizonU(int32_t horizon)`
- **目的**: 画像の水平フリップ機能を有効/無効化
- **パラメータ**: 0=無効、1=有効
- **対応製品**: Tiny4K, Tiny2シリーズ, Meetシリーズ

#### `cameraSetMirrorFlipR(int32_t mirror_flip)`
- **目的**: ビデオ画像をミラー/フリップ
- **モード**: MirrorOffFlipOff, MirrorOnFlipOff, MirrorOffFlipOn, MirrorOnFlipOn, MirrorOnSingle, MirrorOffSingle, FlipOnSingle, FlipOffSingle
- **対応製品**: Tail Air

#### `cameraGetMirrorFlipR(int32_t &mirror_flip)`
- **目的**: ビデオ画像のミラー・フリップ状態を取得
- **対応製品**: Tail Air

### 回転設定

#### `cameraSetRotationDegree(int32_t rotation)`
- **目的**: デバイスの回転度を設定
- **回転**: Rotation90, Rotation180, Rotation270
- **対応製品**: Tail Air

#### `cameraGetRotationDegree(int32_t &rotation)`
- **目的**: デバイスの回転状態を取得
- **対応製品**: Tail Air

### スリープ設定

#### `cameraSetSuspendTimeU(int32_t sleep_time)`
- **目的**: デバイスの自動スリープ時間を設定
- **範囲**: -65535〜65535秒（負の値または0で自動スリープ無効）
- **対応製品**: Tiny, Tiny4K, Tiny2シリーズ, Meetシリーズ

#### `cameraSetDisableSleepWithoutStreamU(bool enable)`
- **目的**: ビデオストリーム出力がない場合の自動スリープを無効化
- **対応製品**: Meetシリーズ

#### `cameraSetMicrophoneDuringSleepU(int32_t microphone)`
- **目的**: スリープモード中のマイク機能を有効/無効化
- **パラメータ**: 0=無効、1=有効
- **対応製品**: Tiny4K, Tiny2シリーズ, Meetシリーズ

### オーディオ設定

#### `cameraSetAudioAutoGainU(bool enabled)`
- **目的**: オーディオオートゲインコントロールを有効/無効化
- **対応製品**: Tiny2シリーズ

### LED制御

#### `cameraSetLedPatternU(bool enabled)`
- **目的**: ゾーントラッキング/ハンドトラッキング設定前後に特殊LEDパターンを開く/閉じる
- **対応製品**: Tiny2

#### `cameraSetLedCtrlU(bool enabled)`
- **目的**: LEDインジケーターを制御
- **対応製品**: TinySE

### 電源制御

#### `cameraSetPowerCtrlActionR(DevPowerCtrlActionType action)`
- **目的**: デバイスの再開、サスペンド、再起動、電源オフなど
- **アクション**: Resume（再開）, Suspend（一時停止）, Reboot（再起動）, PowerOff（電源オフ）, MediaExit（メディア終了）
- **対応製品**: Tail Air

### ブザー制御

#### `sysMgSetBuzzerEnabledR()` / `sysMgSetBuzzerDisabledR()`
- **目的**: ブザーを有効/無効化
- **対応製品**: Tail Air

#### `sysMgGetBuzzerEnabledR(DevBuzzerStatus &buzzer_status)`
- **目的**: ブザー有効状態を取得
- **対応製品**: Tail Air

### デバイス名

#### `sysMgSetDeviceNameR(const std::string &name)`
- **目的**: デバイス名を設定
- **対応製品**: Tail Air

#### `sysMgGetDeviceNameR(std::string &name)`
- **目的**: デバイス名を取得
- **対応製品**: Tail Air

### ファクトリーリセット

#### `cameraSetRestoreFactorySettingsR()`
- **目的**: デバイスを工場出荷時設定にリセット
- **対応製品**: 全製品

### バージョン情報

#### `devVersion()`
- **目的**: デバイスの現在のファームウェアバージョンを取得

#### `uvcVersion()`
- **目的**: デバイスが実装するUVC仕様のバージョンを取得

---

## 製品サポートマトリックス

| 機能カテゴリ | Tiny/Tiny4K | Tiny2/TinySE | Meet/Meet4K | Tail Air | Tail2 |
|------------|-------------|--------------|-------------|----------|-------|
| **基本カメラ制御** | ✓ | ✓ | ✓ | ✓ | ✓ |
| **ズーム制御** | ✓ | ✓ | ✓ | ✓✓✓ | ✓✓✓ |
| **フォーカス制御** | ✓ | ✓ | ✓ | ✓✓✓ | ✓✓✓ |
| **AIトラッキング** | ✓ | ✓✓ | ✓ | ✓✓✓ | ✓✓✓ |
| **ゾーントラッキング** | - | ✓✓ | - | ✓ | ✓ |
| **ジェスチャー制御** | - | ✓✓ | - | ✓✓ | ✓✓ |
| **音声制御** | - | ✓✓ | - | - | - |
| **バーチャル背景** | - | - | ✓✓✓ | - | - |
| **HDR/WDR** | ✓ | ✓ | ✓ | ✓ | ✓ |
| **プリセット位置** | ✓ | ✓✓ | ✓ | ✓✓ | ✓✓ |
| **メディアストリーム制御** | - | - | - | ✓ | ✓✓✓ |

✓✓✓: フル機能サポート | ✓✓: 拡張サポート | ✓: 基本サポート | -: 非対応

---

## 注意事項

### 共通の注意点

1. **モード切り替えの待機時間**
   - HDR/WDR、メディアモードの切り替えは3秒以上の間隔を空けることを推奨

2. **AIトラッキングとジンバル制御の競合**
   - AIスマートトラッキングが有効な場合、ジンバルは常にAIによって制御される
   - 手動制御を行う場合は、AIトラッキングを無効にする必要がある

3. **同期/非同期API**
   - 末尾に`R`が付く関数は、同期（Block）または非同期（NonBlock）で実行可能
   - 末尾に`U`が付く関数は、UVC経由での一方向通信（応答なし）

4. **デバイスステータス**
   - `cameraStatus()`は2-3秒のラグがある可能性がある
   - リアルタイム性が必要な場合は、コールバックを使用することを推奨

### 製品別の特徴

- **Tiny/Tiny4K**: 基本的なカメラ制御とAI機能
- **Tiny2シリーズ**: 高度なAIトラッキング、ゾーントラッキング、ジェスチャー制御、音声制御
- **TinySE**: Tiny2に準ずる機能
- **Meet/Meet4K**: 会議用途向け、バーチャル背景、オートフレーミング
- **Tail Air**: 最も高度な機能（プロフェッショナル向け）
- **Tail2**: 最新世代、メディアストリーム制御、エンコード設定

---

## 参考資料

- OBSBOT SDK ヘッダーファイル: `libdev_v2.1.0_7/include/dev/dev.hpp`
- OBSBOT SDK サンプルコード: `libdev_v2.1.0_7/OBSBOT_Sample/main.cpp`

---

このドキュメントは OBSBOT SDK v2.1.0_7 を基に作成されました。
