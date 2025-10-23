# OBSBOT SDK Features List

A comprehensive list of all control features available in OBSBOT SDK v2.1.0_7.

---

## Table of Contents

1. [Pan-Tilt Control](#1-pan-tilt-control)
2. [Gimbal Control](#2-gimbal-control)
3. [Zoom Control](#3-zoom-control)
4. [Focus Control](#4-focus-control)
5. [AI Tracking Modes](#5-ai-tracking-modes)
6. [Camera Settings](#6-camera-settings)
7. [FOV (Field of View) Settings](#7-fov-field-of-view-settings)
8. [Preset Positions](#8-preset-positions)
9. [HDR/WDR Settings](#9-hdrwdr-settings)
10. [Media Modes](#10-media-modes)
11. [Device Status Queries](#11-device-status-queries)
12. [Gesture Control](#12-gesture-control)
13. [Voice Control](#13-voice-control)
14. [Other Configuration Features](#14-other-configuration-features)

---

## 1. Pan-Tilt Control

### Relative Movement Control

#### `cameraSetPanTiltRelative(double pan_speed, double tilt_speed)`
- **Purpose**: Set relative movement speed of video preview in pan and tilt axes
- **Parameters**:
  - `pan_speed`: Pan speed, range: -1.0 to 1.0
  - `tilt_speed`: Tilt speed, range: -1.0 to 1.0
- **Supported Products**: Meet, Meet4K, Meet2, MeetSE

### Absolute Position Control

#### `cameraSetPanTiltAbsolute(double pan_deg, double tilt_deg)`
- **Purpose**: Set absolute movement position of video preview in pan and tilt axes
- **Parameters**:
  - `pan_deg`: Pan angle, range: -1.0 to 1.0
  - `tilt_deg`: Tilt angle, range: -1.0 to 1.0
- **Supported Products**: Meet, Meet4K, Meet2, MeetSE

---

## 2. Gimbal Control

### Gimbal Speed Control

#### `aiSetGimbalSpeedCtrlR(double pitch, double pan, double roll)`
- **Purpose**: Set gimbal rotation speed, can be set to 0 to stop the gimbal
- **Parameters**:
  - `pitch`: Pitch speed, range: -90 to 90
  - `pan`: Pan speed, range: -180 to 180
  - `roll`: Roll speed, range: -180 to 180 (currently not used)
- **Note**: When AI smart tracking is enabled, gimbal is always controlled by AI
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

#### `gimbalSpeedCtrlR(double pitch, double pan, double roll)`
- **Purpose**: Set gimbal speed (AI tracking must be disabled)
- **Parameters**:
  - `pitch`: Pitch speed, range: -90 to 90
  - `pan`: Pan speed, range: -180 to 180
  - `roll`: Roll speed, range: -90 to 90 (currently not used, set to 0)
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

#### `aiSetGimbalStop()`
- **Purpose**: Stop the gimbal
- **Supported Products**: Tiny2 series, Tail Air

### Gimbal Position Control

#### `aiSetGimbalMotorAngleR(float pitch, float yaw, float roll)`
- **Purpose**: Move gimbal to specified motor angle
- **Parameters**:
  - `pitch`: Target pitch angle, range: -90 to 90
  - `yaw`: Target yaw angle, range: -180 to 180
  - `roll`: Target roll angle, range: -180 to 180 (currently not used, set to 0)
- **Supported Products**: Tiny2 series, Tail Air

#### `gimbalSetSpeedPositionR(float roll, float pitch, float yaw, float s_roll, float s_pitch, float s_yaw)`
- **Purpose**: Set gimbal target position and reference speed for moving to target position
- **Parameters**:
  - `roll`: Target position for roll (not used)
  - `pitch`: Target position for pitch, range: -90 to 90
  - `yaw`: Target position for yaw, range: -120 to 120
  - `s_roll`: Roll reference speed, range: -90 to 90
  - `s_pitch`: Pitch reference speed, range: -90 to 90
  - `s_yaw`: Yaw reference speed, range: -90 to 90
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

### Gimbal Reset

#### `gimbalRstPosR()` / `aiResetGimbalR()`
- **Purpose**: Reset gimbal to zero position (center)
- **Note**: When AI smart tracking is enabled, gimbal is always controlled by AI
  - Tiny/Tiny4K: Use `aiSetTargetSelectR(false)` to disable AI
  - Tiny2/Tail Air: Use `cameraSetAiModeU(Device::AiWorkModeNone)` to disable AI
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

### Gimbal State Retrieval

#### `aiGetGimbalStateR(AiGimbalStateInfo *gim_info, ...)`
- **Purpose**: Get current gimbal state info synchronously or asynchronously
- **Return Value**: AiGimbalStateInfo structure (Euler angles, motor angles, angular velocities for roll/pitch/yaw)
- **Supported Products**: Tiny2 series, Tail Air

#### `gimbalGetAttitudeInfoR(float xyz[3], ...)`
- **Purpose**: Get gimbal motor angles synchronously or asynchronously
- **Parameters**: xyz array (receives roll, pitch, pan angles in degrees)
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

### Gimbal Parameters

#### `aiSetGimbalSpeedR(float speed)`
- **Purpose**: Set gimbal speed
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

#### `aiSetGimbalParaR(DevGimbalParaType type, ...)`
- **Purpose**: Set gimbal function parameters
- **Parameter Types**:
  - `PresetSpeed`: Preset speed (Tail Air supported)
  - `PanGainAdaptive`: Pan gain adaptive
  - `PanGainValue`: Pan gain custom value
  - `PitchGainAdaptive`: Pitch gain adaptive
  - `PitchGainValue`: Pitch gain custom value
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

#### `aiGetGimbalParaR(DevGimbalParaType type, ...)`
- **Purpose**: Get gimbal function parameters
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

---

## 3. Zoom Control

### Basic Zoom Functions

#### `cameraSetZoomAbsoluteR(float zoom)`
- **Purpose**: Set camera zoom level
- **Parameters**: zoom (normalized value from 1.0 to 2.0)
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Meet, Meet4K, Tail Air

#### `cameraGetZoomAbsoluteR(float &zoom)`
- **Purpose**: Get normalized zoom value
- **Return Value**: Zoom value from 1.0 to 2.0
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Meet, Meet4K, Tail Air

#### `cameraGetRangeZoomAbsoluteR(UvcParamRange &range)`
- **Purpose**: Get zoom parameter range (min, max, step values)
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Meet, Meet4K, Tail Air

### Advanced Zoom Functions

#### `cameraSetZoomWithSpeedAbsoluteR(uint32_t zoom_ratio, uint32_t zoom_speed)`
- **Purpose**: Set zoom ratio and zoom speed
- **Parameters**:
  - `zoom_ratio`: Target zoom ratio (actual value × 100)
  - `zoom_speed`: 0=default speed, 1-10=speed value (higher is faster), 255=maximum speed
- **Supported Products**: Tiny2, Tail Air

#### `cameraSetZoomWithSpeedRelativeR(uint32_t zoom_step, uint32_t zoom_speed, bool step_mode, bool in_out)`
- **Purpose**: Set zoom speed, direction, and mode in relative mode
- **Parameters**:
  - `zoom_step`: Zoom step (only valid when step_mode=true)
  - `zoom_speed`: Zoom speed
  - `step_mode`: true=step mode, false=continuous mode
  - `in_out`: true=zoom in, false=zoom out
- **Supported Products**: Tail Air

#### `cameraSetZoomStopR()`
- **Purpose**: Stop zoom immediately
- **Supported Products**: Tail Air

### AI Auto Zoom

#### `aiSetAiAutoZoomR(bool enabled)`
- **Purpose**: Enable/disable AI auto zoom feature
- **Supported Products**: Tiny2 series, Tail Air

#### `aiSetTargetZoomTypeR(DevTargetZoomType zoom_type, DevCustomizedZoomType customized_zoom_type)`
- **Purpose**: Set zoom type for current target
- **Zoom Types**: Normal, FullBody, HalfBody, CloseUp, Customized, GroupHeadless, GroupLowerBody, Adaptive
- **Supported Products**: Tiny2 series, Tail Air

---

## 4. Focus Control

### Auto Focus Modes

#### `cameraSetAutoFocusModeR(DevAutoFocusType focus_type)`
- **Purpose**: Set auto focus type
- **Focus Types**:
  - `AutoSelect`: Auto select
  - `AFC`: Continuous Auto Focus
  - `AFS`: Single Auto Focus
  - `MF`: Manual Focus
- **Supported Products**: Tail Air

#### `cameraGetAutoFocusModeR(DevAutoFocusType &focus_type)`
- **Purpose**: Get current auto focus type
- **Supported Products**: Tail Air

### Manual Focus

#### `cameraSetFocusPosR(int32_t focus_pos)`
- **Purpose**: Set focus motor position
- **Parameters**: focus_pos (0 to 100)
- **Supported Products**: Tail Air

#### `cameraGetFocusPosR(int32_t &focus_pos)`
- **Purpose**: Get current focus motor position
- **Supported Products**: Tail Air

### Face Focus

#### `cameraSetFaceFocusR(bool enable)`
- **Purpose**: Enable/disable face focus feature
- **Supported Products**: All products

#### `cameraSetFaceAER(int32_t face_ae)`
- **Purpose**: Enable/disable face recognition auto exposure
- **Parameters**: 0=disable, 1=enable
- **Supported Products**: All products

### Continuous Auto Focus Settings

#### `cameraSetAFCTrackModeR(DevAFCType afc_type)`
- **Purpose**: Set continuous auto focus tracking type
- **AFC Types**: Center, Face, AiObject, Foreground
- **Supported Products**: Tail Air

#### `cameraGetAFCTrackModeR(DevAFCType &afc_type)`
- **Purpose**: Get current continuous auto focus type
- **Supported Products**: Tail Air

---

## 5. AI Tracking Modes

### Tracking Mode Settings

#### `aiSetAiTrackModeEnabledR(AiTrackModeType ai_mode, bool enabled)`
- **Purpose**: Enable/disable AI tracking mode
- **Modes**:
  - `AiTrackNormal`: Normal mode
  - `AiTrackHumanNormal`: Human normal mode
  - `AiTrackHumanFullBody`: Human full body mode
  - `AiTrackHumanHalfBody`: Human half body mode
  - `AiTrackHumanCloseUp`: Human close-up mode
  - `AiTrackHumanAutoView`: Human auto view mode
  - `AiTrackAnimalNormal`: Animal normal mode
  - `AiTrackAnimalCloseUp`: Animal close-up mode
  - `AiTrackAnimalAutoView`: Animal auto view mode
  - `AiTrackGroup`: Group mode
- **Supported Products**: Tail Air

#### `aiSetTrackingModeR(AiVerticalTrackType mode)`
- **Purpose**: Set AI smart tracking mode
- **Modes**:
  - `AiVTrackStandard`: Standard mode
  - `AiVTrackHeadroom`: Headroom mode
  - `AiVTrackMotion`: Motion mode
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

### Tracking Speed

#### `aiSetTrackSpeedTypeR(AiTrackSpeedType track_type)`
- **Purpose**: Set AI tracking speed
- **Speed Types**: Lazy, Slow, Standard, Fast, Crazy, Auto
- **Supported Products**: Tail Air

### Target Selection

#### `aiSelectTargetInBoxR(int32_t left, int32_t top, int32_t width, int32_t height, ...)`
- **Purpose**: Select tracking target within specified box
- **Parameters**: Box coordinates and size (normalized values 0 to 10000)
- **Supported Products**: Tiny2 series, Tail Air

#### `aiSelectBiggestTargetR(AiTargetType target_type)`
- **Purpose**: Automatically select and track the largest target in image
- **Target Types**: Auto, Person, Cat, Dog, Horse, Animal
- **Supported Products**: Tiny2 series, Tail Air

#### `aiSelectCenterTargetR(AiTargetType target_type)`
- **Purpose**: Select and track the center target in image
- **Supported Products**: Tiny2 series, Tail Air

#### `aiCancelSelectTargetR()`
- **Purpose**: Cancel selected tracking target
- **Supported Products**: Tiny2 series, Tail Air

### Zone Tracking

#### `aiSetZoneTrackStateR(bool enabled)`
- **Purpose**: Enable/disable zone tracking feature
- **Supported Products**: Tiny2 series, Tail Air

#### `aiSetLimitedZoneTrackEnabledR(bool enabled)`
- **Purpose**: Enable/disable limited zone tracking area feature
- **Supported Products**: Tiny2 series

#### `aiSetLimitedZoneTrackYawMinR(float deg)` / `aiSetLimitedZoneTrackYawMaxR(float deg)`
- **Purpose**: Set yaw angle range for limited zone tracking
- **Parameters**: Angle in degrees
- **Supported Products**: Tiny2 series

#### `aiSetLimitedZoneTrackPitchMinR(float deg)` / `aiSetLimitedZoneTrackPitchMaxR(float deg)`
- **Purpose**: Set pitch angle range for limited zone tracking
- **Parameters**: Angle in degrees
- **Supported Products**: Tiny2 series

#### `aiSetLimitedZoneTrackAutoSelectR(bool enabled)`
- **Purpose**: Enable/disable automatic target selection for zone-limited tracking
- **Supported Products**: Tiny2 series

### Hand Tracking

#### `aiSetHandTrackTypeR(DevHandTrackType hand_type)`
- **Purpose**: Set hand tracking type (left/right hand)
- **Supported Products**: Tiny2 series, TinySE

---

## 6. Camera Settings

### Exposure Control

#### `cameraSetExposureModeR(int32_t exposure_mode)`
- **Purpose**: Set shutter mode
- **Modes**: Unknown, Manual, AllAuto, AperturePriority, ShutterPriority
- **Supported Products**: Tail Air

#### `cameraSetExposureAbsolute(int32_t shutter_time, bool auto_enabled)`
- **Purpose**: Set shutter value and auto exposure mode
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraGetExposureAbsolute(int32_t &shutter_time, bool &auto_enabled)`
- **Purpose**: Get shutter time and auto exposure mode
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraSetAELockR(bool enabled)`
- **Purpose**: Enable/disable AE (Auto Exposure) lock
- **Supported Products**: Tail Air

### EVBIAS Settings (Exposure Compensation)

#### `cameraSetPAEEvBiasR(int32_t ev_bias)`
- **Purpose**: Set EVBIAS for auto exposure in P gear (aperture priority)
- **Range**: -3.0 to +3.0 (DevAEEvBiasType enum)
- **Supported Products**: Tail Air

#### `cameraSetSAEEvBiasR(int32_t ev_bias)`
- **Purpose**: Set EVBIAS for auto exposure in S gear (shutter priority)
- **Supported Products**: Tail Air

#### `cameraSetAAEEvBiasR(int32_t ev_bias)`
- **Purpose**: Set EVBIAS for auto exposure in A gear (auto)
- **Supported Products**: Tail Air

### ISO Settings

#### `cameraSetISOLimitR(uint32_t min_iso, uint32_t max_iso)`
- **Purpose**: Set maximum and minimum ISO values
- **Supported Products**: Tail Air

#### `cameraGetISOLimitR(uint32_t &min_iso, uint32_t &max_iso)`
- **Purpose**: Get current maximum and minimum ISO values
- **Supported Products**: Tail Air

#### `cameraSetMAEIsoR(int32_t iso)` / `cameraGetMAEIsoR(int32_t &iso)`
- **Purpose**: Set/get auto ISO exposure in M gear (manual)
- **Supported Products**: Tail Air

### White Balance

#### `cameraSetWhiteBalanceR(DevWhiteBalanceType wb_type, int32_t param)`
- **Purpose**: Set white balance parameters
- **WB Types**:
  - Auto, Daylight, Fluorescent, Tungsten
  - Flash, Fine, Cloudy, Manual, and 16 other types (24 types total)
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraGetWhiteBalanceR(DevWhiteBalanceType &wb_type, int32_t &param)`
- **Purpose**: Get current white balance parameters
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraGetWhiteBalanceListR(std::vector<int32_t> &wb_list, int32_t &wb_min, int32_t &wb_max)`
- **Purpose**: Get supported white balance list and range
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

### Anti-Flicker

#### `cameraSetAntiFlickR(int32_t freq)`
- **Purpose**: Set anti-flicker mode
- **Frequency**: Off, 50Hz, 60Hz, Auto
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraGetAntiFlickR(int32_t &freq)`
- **Purpose**: Get current anti-flicker mode
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

### Image Adjustments

#### `cameraSetImageBrightnessR(int32_t brightness)`
- **Purpose**: Set image brightness
- **Range**: 0 to 100
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraSetImageContrastR(int32_t contrast)`
- **Purpose**: Set image contrast
- **Range**: 0 to 100
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraSetImageSaturationR(int32_t saturation)`
- **Purpose**: Set image saturation
- **Range**: 0 to 100
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraSetImageSharpR(int32_t sharp)`
- **Purpose**: Set image sharpness
- **Range**: 0 to 100
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

#### `cameraSetImageHueR(int32_t hue)`
- **Purpose**: Set image hue
- **Range**: 0 to 100
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air, Meet, Meet4K

### Image Style

#### `cameraSetImageStyleR(DevImageStyle style)`
- **Purpose**: Set image style
- **Styles**: Standard, Text, LandScape, Portrait, NightScape, Film, Customer
- **Supported Products**: Tail Air

---

## 7. FOV (Field of View) Settings

#### `cameraSetFovU(FovType fov_type)`
- **Purpose**: Set camera FOV (field of view)
- **FOV Types**:
  - `FovType86`: 86 degrees (wide)
  - `FovType78`: 78 degrees (medium)
  - `FovType65`: 65 degrees (narrow)
- **Supported Products**: Tiny series, Meet series

---

## 8. Preset Positions

### Gimbal Presets

#### `aiGetGimbalPresetListR(DevDataArray *ids, ...)`
- **Purpose**: Get ID list of existing preset positions
- **Supported Products**: Tiny2 series, Tail Air

#### `aiGetGimbalPresetInfoWithIdR(PresetPosInfo *preset_info, int32_t id, ...)`
- **Purpose**: Get preset position information for specified ID
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

#### `aiAddGimbalPresetR(PresetPosInfo *preset_info)`
- **Purpose**: Add preset position (update if already exists)
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air

#### `aiDelGimbalPresetR(int32_t id)`
- **Purpose**: Delete preset position for specified ID
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air

#### `aiTrgGimbalPresetR(int pos_id)`
- **Purpose**: Move gimbal to preset position for specified ID
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

### Boot Presets

#### `aiSetGimbalBootPosR(const PresetPosInfo &preset_info, bool presets_flag)`
- **Purpose**: Set initial position and zoom ratio at boot
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

#### `aiGetGimbalBootPosR(PresetPosInfo *preset_info, ...)`
- **Purpose**: Get gimbal boot information
- **Supported Products**: Tiny, Tiny4K, Tiny2, Tail Air

#### `aiRstGimbalBootPosR()`
- **Purpose**: Reset boot initial position to default value
- **Supported Products**: Tiny2 series, Tail Air

#### `aiTrgGimbalBootPosR(bool reset_mode)`
- **Purpose**: Move gimbal to boot initial position
- **Supported Products**: Tiny2 series, Tail Air

---

## 9. HDR/WDR Settings

#### `cameraSetWdrR(int32_t wdr_mode)`
- **Purpose**: Set WDR/HDR feature
- **Modes**:
  - `DevWdrModeNone`: HDR disabled
  - `DevWdrModeDol2TO1`: HDR enabled
- **Note**: Mode switching takes time; recommended to wait 3+ seconds between changes
- **Supported Products**: Tiny4K, Tiny2 series, Meet series, Tail Air

#### `cameraGetWdrR(int32_t &wdr_mode)`
- **Purpose**: Get current WDR state
- **Supported Products**: Tail Air

#### `cameraGetWdrListR(std::vector<int32_t> &wdr_list)`
- **Purpose**: Get list of supported WDR states
- **Supported Products**: Tail Air

---

## 10. Media Modes

### Media Mode Settings (Meet Products Only)

#### `cameraSetMediaModeU(MediaMode mode)`
- **Purpose**: Set media mode
- **Modes**:
  - `MediaModeNormal`: Normal mode
  - `MediaModeBackground`: Virtual background mode
  - `MediaModeAutoFrame`: Auto framing mode
- **Note**: Mode switching takes time; recommended to wait 3+ seconds between changes
- **Supported Products**: Meet, Meet4K

### Virtual Background (Meet Products Only)

#### `cameraSetBgModeU(MediaBgMode mode)`
- **Purpose**: Set virtual background mode
- **Modes**:
  - `MediaBgModeDisable`: None
  - `MediaBgModeColor`: Green screen mode
  - `MediaBgModeReplace`: Replace mode
  - `MediaBgModeBlur`: Blur mode
- **Supported Products**: Meet, Meet4K

#### `cameraSetBgColorU(MediaBgModeColorType bg_color)`
- **Purpose**: Set background color in green screen mode
- **Colors**: Blue, Green, Red, Black, White
- **Supported Products**: Meet, Meet4K

#### `cameraSetBgBlurLevelU(int32_t level)`
- **Purpose**: Set background blur level
- **Supported Products**: Meet, Meet4K

### Auto Framing (Meet Products Only)

#### `cameraSetAutoFramingModeU(AutoFramingType group_single, AutoFramingType close_upper)`
- **Purpose**: Set auto frame mode
- **Modes**: AutoFrmGroup, AutoFrmSingle, AutoFrmCloseUp, AutoFrmUpperBody
- **Supported Products**: Meet series

### Recording (Tail Air Only)

#### `cameraSetVideoRecordR(uint32_t operation, uint32_t param)`
- **Purpose**: Start/stop video recording
- **Operation**: 0=stop, 1=start
- **Supported Products**: Tail Air

#### `cameraSetRecordResolutionR(DevVideoResType res_type)`
- **Purpose**: Set video resolution for recording
- **Supported Products**: Tail Air

#### `cameraSetRecordSplitSizeR(DevVideoSplitSizeType split_type)`
- **Purpose**: Set video file split size during recording
- **Supported Products**: Tail Air

---

## 11. Device Status Queries

### Status Retrieval

#### `cameraStatus()`
- **Purpose**: Get camera status (may have 2-3 second lag)
- **Return Value**: CameraStatus structure

#### `cameraGetCameraStatusU(CameraStatus &camera_status)`
- **Purpose**: Get camera status information
- **Supported Products**: All products

#### `aiGetAiStatusR(AiStatus *ai_status, ...)`
- **Purpose**: Get current AI status synchronously/asynchronously
- **Supported Products**: Tiny, Tiny4K, Tiny2 series

### Status Callbacks

#### `setDevStatusCallbackFunc(DevStatusCallback callback, void *param)`
- **Purpose**: Register DevStatusCallback (pushes device status every 2-3 seconds)

#### `enableDevStatusCallback(bool enabled)`
- **Purpose**: Enable/disable DevStatusCallback

### Operating State

#### `cameraSetDevRunStatusR(DevStatus type)`
- **Purpose**: Set device operating state
- **States**:
  - `DevStatusRun`: Normal mode
  - `DevStatusSleep`: Sleep mode
  - `DevStatusPrivacy`: Privacy mode (stream stopped)
- **Supported Products**: All products

---

## 12. Gesture Control

### Gesture Features

#### `aiSetGestureCtrlR(bool flag)`
- **Purpose**: Turn entire gesture control on/off
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Tail Air

#### `aiSetGestureCtrlIndividualR(int32_t gesture, bool flag)`
- **Purpose**: Turn specified gesture feature on/off
- **Gesture Types**:
  - `0`: Gesture target
  - `1`: Gesture zoom
  - `2`: Gesture dynamic zoom - Tiny2 series, Tail Air
  - `3`: Gesture dynamic zoom direction - Tiny2 series, Tail Air
  - `4`: Gesture record - Tail Air
- **Supported Products**: Tiny2 series, Tail Air

### Gesture Parameters

#### `aiSetGestureParaR(DevGestureParaType type, ...)`
- **Purpose**: Set gesture function parameters
- **Parameter Types**:
  - `Gesture`: Overall gesture function
  - `TargetSelection`: Target selection gesture
  - `Zoom`: Zoom gesture
  - `DynamicZoom`: Dynamic zoom gesture
  - `Record`: Record gesture
  - `Snapshot`: Snapshot gesture
  - `Rolling`: Rolling gesture
  - `Mirror`: Mirror
  - `ZoomFactor`: Hand pose zoom ratio

#### `aiGetGestureParaR(DevGestureParaType type, ...)`
- **Purpose**: Get gesture function parameters

### Gesture Tracking Parameters

#### `aiSetGestureTrackParaR(DevGestureTrackParaType type, ...)`
- **Purpose**: Set gesture tracking function parameters
- **Parameter Types**:
  - `PanMin` / `PanMax`: Pan limits
  - `PitchMin` / `PitchMax`: Pitch limits
  - `HandType`: Left/right hand tracking
  - `TrackSpeed`: Tracking speed
  - `PanEnabled` / `PitchEnabled`: Enable pan/pitch
  - `RestSeconds`: Seconds before returning to initial position after reaching boundary

#### `aiGetGestureTrackParaR(DevGestureTrackParaType type, ...)`
- **Purpose**: Get gesture tracking function parameters

---

## 13. Voice Control

### Voice Commands (Tiny2 Series Only)

#### `cameraSetVoiceCtrlU(AudioCtrlCmdType cmd, int32_t state)`
- **Purpose**: Enable/disable specified voice control commands, select language, set zoom factor
- **Command Types**:
  - `AudioCtrlHiTiny`: Hi Tiny (wake command)
  - `AudioCtrlSleepTiny`: Sleep Tiny (sleep command)
  - `AudioCtrlTrack`: Track Me (start tracking)
  - `AudioCtrlUnlock`: Unlock Me (unlock tracking)
  - `AudioCtrlZoomIn`: Zoom In Close
  - `AudioCtrlZoomOut`: Zoom Out Further
  - `AudioCtrlPreset`: Preset Position 1/2/3
  - `AudioCtrlZoomFactor`: For zoom factor setting (0 to 100)
  - `AudioCtrlLanguage`: Voice control language setting
- **Languages**: Chinese (ZhCN), English (EnUS)
- **Supported Products**: Tiny2 series

---

## 14. Other Configuration Features

### Image Flip & Mirror

#### `cameraSetImageFlipHorizonU(int32_t horizon)`
- **Purpose**: Enable/disable horizontal flip feature
- **Parameters**: 0=disable, 1=enable
- **Supported Products**: Tiny4K, Tiny2 series, Meet series

#### `cameraSetMirrorFlipR(int32_t mirror_flip)`
- **Purpose**: Mirror/flip video image
- **Modes**: MirrorOffFlipOff, MirrorOnFlipOff, MirrorOffFlipOn, MirrorOnFlipOn, MirrorOnSingle, MirrorOffSingle, FlipOnSingle, FlipOffSingle
- **Supported Products**: Tail Air

#### `cameraGetMirrorFlipR(int32_t &mirror_flip)`
- **Purpose**: Get video image mirror/flip state
- **Supported Products**: Tail Air

### Rotation Settings

#### `cameraSetRotationDegree(int32_t rotation)`
- **Purpose**: Set device rotation degree
- **Rotation**: Rotation90, Rotation180, Rotation270
- **Supported Products**: Tail Air

#### `cameraGetRotationDegree(int32_t &rotation)`
- **Purpose**: Get device rotation state
- **Supported Products**: Tail Air

### Sleep Settings

#### `cameraSetSuspendTimeU(int32_t sleep_time)`
- **Purpose**: Set device auto sleep time
- **Range**: -65535 to 65535 seconds (negative value or 0 disables auto sleep)
- **Supported Products**: Tiny, Tiny4K, Tiny2 series, Meet series

#### `cameraSetDisableSleepWithoutStreamU(bool enable)`
- **Purpose**: Disable auto sleep when video stream output is absent
- **Supported Products**: Meet series

#### `cameraSetMicrophoneDuringSleepU(int32_t microphone)`
- **Purpose**: Enable/disable microphone function during sleep mode
- **Parameters**: 0=disable, 1=enable
- **Supported Products**: Tiny4K, Tiny2 series, Meet series

### Audio Settings

#### `cameraSetAudioAutoGainU(bool enabled)`
- **Purpose**: Enable/disable audio auto gain control
- **Supported Products**: Tiny2 series

### LED Control

#### `cameraSetLedPatternU(bool enabled)`
- **Purpose**: Open/close special LED pattern before/after zone tracking/hand tracking settings
- **Supported Products**: Tiny2

#### `cameraSetLedCtrlU(bool enabled)`
- **Purpose**: Control LED indicator
- **Supported Products**: TinySE

### Power Control

#### `cameraSetPowerCtrlActionR(DevPowerCtrlActionType action)`
- **Purpose**: Resume, suspend, reboot, power off device, etc.
- **Actions**: Resume, Suspend, Reboot, PowerOff, MediaExit
- **Supported Products**: Tail Air

### Buzzer Control

#### `sysMgSetBuzzerEnabledR()` / `sysMgSetBuzzerDisabledR()`
- **Purpose**: Enable/disable buzzer
- **Supported Products**: Tail Air

#### `sysMgGetBuzzerEnabledR(DevBuzzerStatus &buzzer_status)`
- **Purpose**: Get buzzer enabled state
- **Supported Products**: Tail Air

### Device Name

#### `sysMgSetDeviceNameR(const std::string &name)`
- **Purpose**: Set device name
- **Supported Products**: Tail Air

#### `sysMgGetDeviceNameR(std::string &name)`
- **Purpose**: Get device name
- **Supported Products**: Tail Air

### Factory Reset

#### `cameraSetRestoreFactorySettingsR()`
- **Purpose**: Reset device to factory settings
- **Supported Products**: All products

### Version Information

#### `devVersion()`
- **Purpose**: Get device's current firmware version

#### `uvcVersion()`
- **Purpose**: Get UVC specification version implemented by device

---

## Product Support Matrix

| Feature Category | Tiny/Tiny4K | Tiny2/TinySE | Meet/Meet4K | Tail Air | Tail2 |
|-----------------|-------------|--------------|-------------|----------|-------|
| **Basic Camera Control** | ✓ | ✓ | ✓ | ✓ | ✓ |
| **Zoom Control** | ✓ | ✓ | ✓ | ✓✓✓ | ✓✓✓ |
| **Focus Control** | ✓ | ✓ | ✓ | ✓✓✓ | ✓✓✓ |
| **AI Tracking** | ✓ | ✓✓ | ✓ | ✓✓✓ | ✓✓✓ |
| **Zone Tracking** | - | ✓✓ | - | ✓ | ✓ |
| **Gesture Control** | - | ✓✓ | - | ✓✓ | ✓✓ |
| **Voice Control** | - | ✓✓ | - | - | - |
| **Virtual Background** | - | - | ✓✓✓ | - | - |
| **HDR/WDR** | ✓ | ✓ | ✓ | ✓ | ✓ |
| **Preset Positions** | ✓ | ✓✓ | ✓ | ✓✓ | ✓✓ |
| **Media Stream Control** | - | - | - | ✓ | ✓✓✓ |

✓✓✓: Full feature support | ✓✓: Extended support | ✓: Basic support | -: Not supported

---

## Important Notes

### Common Considerations

1. **Mode Switching Wait Time**
   - Recommended to wait 3+ seconds between HDR/WDR and media mode switches

2. **AI Tracking and Gimbal Control Conflicts**
   - When AI smart tracking is enabled, gimbal is always controlled by AI
   - Must disable AI tracking for manual control

3. **Synchronous/Asynchronous APIs**
   - Functions ending with `R` can execute in Block or NonBlock mode
   - Functions ending with `U` are one-way UVC communication (no response)

4. **Device Status**
   - `cameraStatus()` may have 2-3 second lag
   - Recommended to use callbacks for real-time requirements

### Product-Specific Features

- **Tiny/Tiny4K**: Basic camera control and AI features
- **Tiny2 series**: Advanced AI tracking, zone tracking, gesture control, voice control
- **TinySE**: Similar features to Tiny2
- **Meet/Meet4K**: Conference-oriented, virtual background, auto framing
- **Tail Air**: Most advanced features (professional use)
- **Tail2**: Latest generation, media stream control, encoding settings

---

## References

- OBSBOT SDK Header File: `libdev_v2.1.0_7/include/dev/dev.hpp`
- OBSBOT SDK Sample Code: `libdev_v2.1.0_7/OBSBOT_Sample/main.cpp`

---

This document was created based on OBSBOT SDK v2.1.0_7.
