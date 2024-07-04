static const char* g_PooledStrings[] = {
	"AcceptInput",
	"AdjustMouseSensitivity",
	"AllowPlayerPickup",
	"CalcMainActivity",
	"CalcView",
	"CalcViewModelView",
	"CanExitVehicle",
	"CanPlayerEnterVehicle",
	"CanPlayerSuicide",
	"ChatText",
	"ChatTextChanged",
	"CheckPassword",
	"ClientSignOnStateChanged",
	"CreateClientsideRagdoll",
	"CreateEntityRagdoll",
	"CreateMove",
	"DoAnimationEvent",
	"DoPlayerDeath",
	"DrawMonitors",
	"DrawPhysgunBeam",
	"EndEntityDriving",
	"EntityEmitSound",
	"EntityFireBullets",
	"PostEntityFireBullets",
	"EntityKeyValue",
	"EntityNetworkedVarChanged",
	"EntityRemoved",
	"EntityTakeDamage",
	"PostEntityTakeDamage",
	"FindUseEntity",
	"FinishChat",
	"FinishMove",
	"GUIMouseDoublePressed",
	"GUIMousePressed",
	"GUIMouseReleased",
	"GetFallDamage",
	"GetGameDescription",
	"GetMotionBlurValues",
	"GetPreferredCarryAngles",
	"GetTeamColor",
	"GravGunOnDropped",
	"GravGunOnPickedUp",
	"GravGunPickupAllowed",
	"GravGunPunt",
	"HUDAmmoPickedUp",
	"HUDDrawScoreBoard",
	"HUDItemPickedUp",
	"HUDPaint",
	"HUDPaintBackground",
	"HUDShouldDraw",
	"HUDWeaponPickedUp",
	"InitPostEntity",
	"InputMouseApply",
	"KeyPress",
	"KeyRelease",
	"Move",
	"NeedsDepthPass",
	"NetworkEntityCreated",
	"NetworkIDValidated",
	"NotifyShouldTransmit",
	"OnAchievementAchieved",
	"OnChatTab",
	"OnCrazyPhysics",
	"OnDamagedByExplosion",
	"OnEntityCreated",
	"OnEntityWaterLevelChanged",
	"OnNPCKilled",
	"OnPhysgunFreeze",
	"OnPhysgunReload",
	"OnPlayerChat",
	"OnPlayerHitGround",
	"PlayerChangedTeam",
	"OnViewModelChanged",
	"PhysgunDrop",
	"PhysgunPickup",
	"OnPhysgunPickup",
	"PlayerAuthed",
	"PlayerBindPress",
	"PlayerButtonDown",
	"PlayerButtonUp",
	"PlayerCanHearPlayersVoice",
	"PlayerCanPickupItem",
	"PlayerCanPickupWeapon",
	"PlayerCanSeePlayersChat",
	"PlayerConnect",
	"PlayerDeath",
	"PlayerDeathSound",
	"PlayerDeathThink",
	"PlayerDroppedWeapon",
	"PlayerDisconnected",
	"PlayerDriveAnimate",
	"PlayerEndVoice",
	"PlayerEnteredVehicle",
	"PlayerFootstep",
	"PlayerHurt",
	"PlayerInitialSpawn",
	"PlayerLeaveVehicle",
	"PlayerNoClip",
	"PlayerPostThink",
	"PlayerSay",
	"PlayerSelectSpawn",
	"PlayerShouldTakeDamage",
	"PlayerShouldTaunt",
	"PlayerSilentDeath",
	"PlayerSpawn",
	"PlayerSpray",
	"PlayerStartTaunt",
	"PlayerStartVoice",
	"PlayerStepSoundTime",
	"PlayerSwitchFlashlight",
	"PlayerSwitchWeapon",
	"PlayerTick",
	"PlayerTraceAttack",
	"PlayerUse",
	"PostCleanupMap",
	"PostDraw2DSkyBox",
	"PostDrawEffects",
	"PostDrawHUD",
	"PostDrawOpaqueRenderables",
	"PostDrawSkyBox",
	"PostDrawTranslucentRenderables",
	"PostDrawViewModel",
	"PostPlayerDeath",
	"PostPlayerDraw",
	"PostRender",
	"PostRenderVGUI",
	"PreCleanupMap",
	"PreDrawEffects",
	"PreDrawHUD",
	"PreDrawOpaqueRenderables",
	"PreDrawSkyBox",
	"PreDrawTranslucentRenderables",
	"PreDrawViewModel",
	"PreDrawViewModels",
	"PrePlayerDraw",
	"PreRender",
	"PreventScreenClicks",
	"PropBreak",
	"RenderScene",
	"RenderScreenspaceEffects",
	"Restored",
	"Saved",
	"ScaleNPCDamage",
	"ScalePlayerDamage",
	"ScoreboardHide",
	"ScoreboardShow",
	"SetupMove",
	"SetupPlayerVisibility",
	"SetupSkyboxFog",
	"SetupWorldFog",
	"ShouldCollide",
	"ShouldDrawLocalPlayer",
	"ShouldDrawParticles",
	"ShowHelp",
	"ShowSpare1",
	"ShowSpare2",
	"ShowTeam",
	"ShutDown",
	"SpawniconGenerated",
	"StartChat",
	"StartCommand",
	"StartEntityDriving",
	"Think",
	"Tick",
	"TranslateActivity",
	"UpdateAnimation",
	"VGUIMousePressAllowed",
	"VGUIMousePressed",
	"VehicleMove",
	"ViewPunch",
	"WeaponEquip",
	"PlayerAmmoChanged",
	"PlayerClassChanged",
	"PlayerHandleAnimEvent",
	"PlayerFireAnimationEvent",
	"OnPlayerPhysicsPickup",
	"OnPlayerPhysicsDrop",
	"OnPlayerJump",
	"OnCloseCaptionEmit",
	"HandlePlayerArmorReduction",
	"OnChangeActivity",
	"BehaveUpdate",
	"Blocked",
	"BodyUpdate",
	"Deploy",
	"DoImpactEffect",
	"Draw",
	"DrawTranslucent",
	"EndTouch",
	"Equip",
	"EquipAmmo",
	"ExpressionFinished",
	"FireAnimationEvent",
	"FreezeMovement",
	"GetAttackSpread",
	"GetCapabilities",
	"GetRelationship",
	"GetRenderMesh",
	"HandleAnimEvent",
	"Holster",
	"ImpactTrace",
	"KeyValue",
	"OnCondition",
	"OnContact",
	"OnIgnite",
	"OnInjured",
	"OnKilled",
	"OnTraceAttack",
	"OnLandOnGround",
	"OnLeaveGround",
	"OnNavAreaChanged",
	"OnOtherKilled",
	"OnStuck",
	"OnTakeDamage",
	"OnUnStuck",
	"PassesDamageFilter",
	"PassesFilter",
	"PassesTriggerFilters",
	"PhysicsCollide",
	"PhysicsSimulate",
	"PhysicsUpdate",
	"RunEngineTask",
	"StartEngineSchedule",
	"StartEngineTask",
	"StartTouch",
	"TestCollision",
	"Touch",
	"UpdateTransmitState",
	"Use",
	"IsJumpLegal",
	"CalcWeaponProficiency",
	"GetShadowCastDirection",
	"OnEntitySight",
	"OnEntitySightLost",
	"OnMovementFailed",
	"OnMovementComplete",
	"OnActiveWeaponChanged",
	"RunAI",
	"OnTaskFailed",
	"OverrideMove",
	"OverrideMoveFacing",
	"TranslateSchedule",
	"OnStateChange",
	"CustomAmmoDisplay",
	"DrawHUD",
	"DrawHUDBackground",
	"DrawWorldModel",
	"DrawWorldModelTranslucent",
	"DrawWeaponSelection",
	"DoDrawCrosshair",
	"ShouldDrawViewModel",
	"GetTracerOrigin",
	"OnDrop",
	"OnRemove",
	"PrimaryAttack",
	"SecondaryAttack",
	"Reload",
	"RenderScreen",
	"ViewModelDrawn",
	"TranslateFOV",
	"SetWeaponHoldType",
	"CanBePickedUpByNPCs",
	"NPCShoot_Primary",
	"NPCShoot_Secondary",
	"GetNPCRestTimes",
	"GetNPCBurstSettings",
	"GetNPCBulletSpread",
	"ShouldDropOnDie",
	"ActionSignal",
	"AllowInput",
	"ApplySchemeSettings",
	"ConsoleMessage",
	"DoClick",
	"OnActivate",
	"OnBeginLoadingDocument",
	"OnCallback",
	"OnChangeAddressBar",
	"OnChangeTargetURL",
	"OnChangeTitle",
	"OnChildViewCreated",
	"OnCursorEntered",
	"OnCursorExited",
	"OnCursorMoved",
	"OnDeactivate",
	"OnDeletion",
	"OnDocumentReady",
	"OnFinishLoadingDocument",
	"OnFocusChanged",
	"OnGetFocus",
	"OnKeyCodePressed",
	"OnKeyCodeReleased",
	"OnKeyCodeTyped",
	"OnLoseFocus",
	"OnMousePressed",
	"OnMouseReleased",
	"OnMouseWheeled",
	"OnScreenSizeChanged",
	"OnSizeChanged",
	"OnTextChanged",
	"OnTextClicked",
	"PerformLayout",
	"ResourceLoaded",
	"TestHover",
	"DTVar_ReceiveProxyGL",
	"Float",
	"Int",
	"Bool",
	"Vector",
	"Angle",
	"String",
	"Entity"
};

// Definitions
#define LUA_POOLEDSTRING_AcceptInput 0
#define LUA_POOLEDSTRING_AdjustMouseSensitivity 1
#define LUA_POOLEDSTRING_AllowPlayerPickup 2
#define LUA_POOLEDSTRING_CalcMainActivity 3
#define LUA_POOLEDSTRING_CalcView 4
#define LUA_POOLEDSTRING_CalcViewModelView 5
#define LUA_POOLEDSTRING_CanExitVehicle 6
#define LUA_POOLEDSTRING_CanPlayerEnterVehicle 7
#define LUA_POOLEDSTRING_CanPlayerSuicide 8
#define LUA_POOLEDSTRING_ChatText 9
#define LUA_POOLEDSTRING_ChatTextChanged 10
#define LUA_POOLEDSTRING_CheckPassword 11
#define LUA_POOLEDSTRING_ClientSignOnStateChanged 12
#define LUA_POOLEDSTRING_CreateClientsideRagdoll 13
#define LUA_POOLEDSTRING_CreateEntityRagdoll 14
#define LUA_POOLEDSTRING_CreateMove 15
#define LUA_POOLEDSTRING_DoAnimationEvent 16
#define LUA_POOLEDSTRING_DoPlayerDeath 17
#define LUA_POOLEDSTRING_DrawMonitors 18
#define LUA_POOLEDSTRING_DrawPhysgunBeam 19
#define LUA_POOLEDSTRING_EndEntityDriving 20
#define LUA_POOLEDSTRING_EntityEmitSound 21
#define LUA_POOLEDSTRING_EntityFireBullets 22
#define LUA_POOLEDSTRING_PostEntityFireBullets 23
#define LUA_POOLEDSTRING_EntityKeyValue 24
#define LUA_POOLEDSTRING_EntityNetworkedVarChanged 25
#define LUA_POOLEDSTRING_EntityRemoved 26
#define LUA_POOLEDSTRING_EntityTakeDamage 27
#define LUA_POOLEDSTRING_PostEntityTakeDamage 28
#define LUA_POOLEDSTRING_FindUseEntity 29
#define LUA_POOLEDSTRING_FinishChat 30
#define LUA_POOLEDSTRING_FinishMove 31
#define LUA_POOLEDSTRING_GUIMouseDoublePressed 32
#define LUA_POOLEDSTRING_GUIMousePressed 33
#define LUA_POOLEDSTRING_GUIMouseReleased 34
#define LUA_POOLEDSTRING_GetFallDamage 35
#define LUA_POOLEDSTRING_GetGameDescription 36
#define LUA_POOLEDSTRING_GetMotionBlurValues 37
#define LUA_POOLEDSTRING_GetPreferredCarryAngles 38
#define LUA_POOLEDSTRING_GetTeamColor 39
#define LUA_POOLEDSTRING_GravGunOnDropped 40
#define LUA_POOLEDSTRING_GravGunOnPickedUp 41
#define LUA_POOLEDSTRING_GravGunPickupAllowed 42
#define LUA_POOLEDSTRING_GravGunPunt 43
#define LUA_POOLEDSTRING_HUDAmmoPickedUp 44
#define LUA_POOLEDSTRING_HUDDrawScoreBoard 45
#define LUA_POOLEDSTRING_HUDItemPickedUp 46
#define LUA_POOLEDSTRING_HUDPaint 47
#define LUA_POOLEDSTRING_HUDPaintBackground 48
#define LUA_POOLEDSTRING_HUDShouldDraw 49
#define LUA_POOLEDSTRING_HUDWeaponPickedUp 50
#define LUA_POOLEDSTRING_InitPostEntity 51
#define LUA_POOLEDSTRING_InputMouseApply 52
#define LUA_POOLEDSTRING_KeyPress 53
#define LUA_POOLEDSTRING_KeyRelease 54
#define LUA_POOLEDSTRING_Move 55
#define LUA_POOLEDSTRING_NeedsDepthPass 56
#define LUA_POOLEDSTRING_NetworkEntityCreated 57
#define LUA_POOLEDSTRING_NetworkIDValidated 58
#define LUA_POOLEDSTRING_NotifyShouldTransmit 59
#define LUA_POOLEDSTRING_OnAchievementAchieved 60
#define LUA_POOLEDSTRING_OnChatTab 61
#define LUA_POOLEDSTRING_OnCrazyPhysics 62
#define LUA_POOLEDSTRING_OnDamagedByExplosion 63
#define LUA_POOLEDSTRING_OnEntityCreated 64
#define LUA_POOLEDSTRING_OnEntityWaterLevelChanged 65
#define LUA_POOLEDSTRING_OnNPCKilled 66
#define LUA_POOLEDSTRING_OnPhysgunFreeze 67
#define LUA_POOLEDSTRING_OnPhysgunReload 68
#define LUA_POOLEDSTRING_OnPlayerChat 69
#define LUA_POOLEDSTRING_OnPlayerHitGround 70
#define LUA_POOLEDSTRING_PlayerChangedTeam 71
#define LUA_POOLEDSTRING_OnViewModelChanged 72
#define LUA_POOLEDSTRING_PhysgunDrop 73
#define LUA_POOLEDSTRING_PhysgunPickup 74
#define LUA_POOLEDSTRING_OnPhysgunPickup 75
#define LUA_POOLEDSTRING_PlayerAuthed 76
#define LUA_POOLEDSTRING_PlayerBindPress 77
#define LUA_POOLEDSTRING_PlayerButtonDown 78
#define LUA_POOLEDSTRING_PlayerButtonUp 79
#define LUA_POOLEDSTRING_PlayerCanHearPlayersVoice 80
#define LUA_POOLEDSTRING_PlayerCanPickupItem 81
#define LUA_POOLEDSTRING_PlayerCanPickupWeapon 82
#define LUA_POOLEDSTRING_PlayerCanSeePlayersChat 83
#define LUA_POOLEDSTRING_PlayerConnect 84
#define LUA_POOLEDSTRING_PlayerDeath 85
#define LUA_POOLEDSTRING_PlayerDeathSound 86
#define LUA_POOLEDSTRING_PlayerDeathThink 87
#define LUA_POOLEDSTRING_PlayerDroppedWeapon 88
#define LUA_POOLEDSTRING_PlayerDisconnected 89
#define LUA_POOLEDSTRING_PlayerDriveAnimate 90
#define LUA_POOLEDSTRING_PlayerEndVoice 91
#define LUA_POOLEDSTRING_PlayerEnteredVehicle 92
#define LUA_POOLEDSTRING_PlayerFootstep 93
#define LUA_POOLEDSTRING_PlayerHurt 94
#define LUA_POOLEDSTRING_PlayerInitialSpawn 95
#define LUA_POOLEDSTRING_PlayerLeaveVehicle 96
#define LUA_POOLEDSTRING_PlayerNoClip 97
#define LUA_POOLEDSTRING_PlayerPostThink 98
#define LUA_POOLEDSTRING_PlayerSay 99
#define LUA_POOLEDSTRING_PlayerSelectSpawn 100
#define LUA_POOLEDSTRING_PlayerShouldTakeDamage 101
#define LUA_POOLEDSTRING_PlayerShouldTaunt 102
#define LUA_POOLEDSTRING_PlayerSilentDeath 103
#define LUA_POOLEDSTRING_PlayerSpawn 104
#define LUA_POOLEDSTRING_PlayerSpray 105
#define LUA_POOLEDSTRING_PlayerStartTaunt 106
#define LUA_POOLEDSTRING_PlayerStartVoice 107
#define LUA_POOLEDSTRING_PlayerStepSoundTime 108
#define LUA_POOLEDSTRING_PlayerSwitchFlashlight 109
#define LUA_POOLEDSTRING_PlayerSwitchWeapon 110
#define LUA_POOLEDSTRING_PlayerTick 111
#define LUA_POOLEDSTRING_PlayerTraceAttack 112
#define LUA_POOLEDSTRING_PlayerUse 113
#define LUA_POOLEDSTRING_PostCleanupMap 114
#define LUA_POOLEDSTRING_PostDraw2DSkyBox 115
#define LUA_POOLEDSTRING_PostDrawEffects 116
#define LUA_POOLEDSTRING_PostDrawHUD 117
#define LUA_POOLEDSTRING_PostDrawOpaqueRenderables 118
#define LUA_POOLEDSTRING_PostDrawSkyBox 119
#define LUA_POOLEDSTRING_PostDrawTranslucentRenderables 120
#define LUA_POOLEDSTRING_PostDrawViewModel 121
#define LUA_POOLEDSTRING_PostPlayerDeath 122
#define LUA_POOLEDSTRING_PostPlayerDraw 123
#define LUA_POOLEDSTRING_PostRender 124
#define LUA_POOLEDSTRING_PostRenderVGUI 125
#define LUA_POOLEDSTRING_PreCleanupMap 126
#define LUA_POOLEDSTRING_PreDrawEffects 127
#define LUA_POOLEDSTRING_PreDrawHUD 128
#define LUA_POOLEDSTRING_PreDrawOpaqueRenderables 129
#define LUA_POOLEDSTRING_PreDrawSkyBox 130
#define LUA_POOLEDSTRING_PreDrawTranslucentRenderables 131
#define LUA_POOLEDSTRING_PreDrawViewModel 132
#define LUA_POOLEDSTRING_PreDrawViewModels 133
#define LUA_POOLEDSTRING_PrePlayerDraw 134
#define LUA_POOLEDSTRING_PreRender 135
#define LUA_POOLEDSTRING_PreventScreenClicks 136
#define LUA_POOLEDSTRING_PropBreak 137
#define LUA_POOLEDSTRING_RenderScene 138
#define LUA_POOLEDSTRING_RenderScreenspaceEffects 139
#define LUA_POOLEDSTRING_Restored 140
#define LUA_POOLEDSTRING_Saved 141
#define LUA_POOLEDSTRING_ScaleNPCDamage 142
#define LUA_POOLEDSTRING_ScalePlayerDamage 143
#define LUA_POOLEDSTRING_ScoreboardHide 144
#define LUA_POOLEDSTRING_ScoreboardShow 145
#define LUA_POOLEDSTRING_SetupMove 146
#define LUA_POOLEDSTRING_SetupPlayerVisibility 147
#define LUA_POOLEDSTRING_SetupSkyboxFog 148
#define LUA_POOLEDSTRING_SetupWorldFog 149
#define LUA_POOLEDSTRING_ShouldCollide 150
#define LUA_POOLEDSTRING_ShouldDrawLocalPlayer 151
#define LUA_POOLEDSTRING_ShouldDrawParticles 152
#define LUA_POOLEDSTRING_ShowHelp 153
#define LUA_POOLEDSTRING_ShowSpare1 154
#define LUA_POOLEDSTRING_ShowSpare2 155
#define LUA_POOLEDSTRING_ShowTeam 156
#define LUA_POOLEDSTRING_ShutDown 157
#define LUA_POOLEDSTRING_SpawniconGenerated 158
#define LUA_POOLEDSTRING_StartChat 159
#define LUA_POOLEDSTRING_StartCommand 160
#define LUA_POOLEDSTRING_StartEntityDriving 161
#define LUA_POOLEDSTRING_Think 162
#define LUA_POOLEDSTRING_Tick 163
#define LUA_POOLEDSTRING_TranslateActivity 164
#define LUA_POOLEDSTRING_UpdateAnimation 165
#define LUA_POOLEDSTRING_VGUIMousePressAllowed 166
#define LUA_POOLEDSTRING_VGUIMousePressed 167
#define LUA_POOLEDSTRING_VehicleMove 168
#define LUA_POOLEDSTRING_ViewPunch 169
#define LUA_POOLEDSTRING_WeaponEquip 170
#define LUA_POOLEDSTRING_PlayerAmmoChanged 171
#define LUA_POOLEDSTRING_PlayerClassChanged 172
#define LUA_POOLEDSTRING_PlayerHandleAnimEvent 173
#define LUA_POOLEDSTRING_PlayerFireAnimationEvent 174
#define LUA_POOLEDSTRING_OnPlayerPhysicsPickup 175
#define LUA_POOLEDSTRING_OnPlayerPhysicsDrop 176
#define LUA_POOLEDSTRING_OnPlayerJump 177
#define LUA_POOLEDSTRING_OnCloseCaptionEmit 178
#define LUA_POOLEDSTRING_HandlePlayerArmorReduction 179
#define LUA_POOLEDSTRING_OnChangeActivity 180
#define LUA_POOLEDSTRING_BehaveUpdate 181
#define LUA_POOLEDSTRING_Blocked 182
#define LUA_POOLEDSTRING_BodyUpdate 183
#define LUA_POOLEDSTRING_Deploy 184
#define LUA_POOLEDSTRING_DoImpactEffect 185
#define LUA_POOLEDSTRING_Draw 186
#define LUA_POOLEDSTRING_DrawTranslucent 187
#define LUA_POOLEDSTRING_EndTouch 188
#define LUA_POOLEDSTRING_Equip 189
#define LUA_POOLEDSTRING_EquipAmmo 190
#define LUA_POOLEDSTRING_ExpressionFinished 191
#define LUA_POOLEDSTRING_FireAnimationEvent 192
#define LUA_POOLEDSTRING_FreezeMovement 193
#define LUA_POOLEDSTRING_GetAttackSpread 194
#define LUA_POOLEDSTRING_GetCapabilities 195
#define LUA_POOLEDSTRING_GetRelationship 196
#define LUA_POOLEDSTRING_GetRenderMesh 197
#define LUA_POOLEDSTRING_HandleAnimEvent 198
#define LUA_POOLEDSTRING_Holster 199
#define LUA_POOLEDSTRING_ImpactTrace 200
#define LUA_POOLEDSTRING_KeyValue 201
#define LUA_POOLEDSTRING_OnCondition 202
#define LUA_POOLEDSTRING_OnContact 203
#define LUA_POOLEDSTRING_OnIgnite 204
#define LUA_POOLEDSTRING_OnInjured 205
#define LUA_POOLEDSTRING_OnKilled 206
#define LUA_POOLEDSTRING_OnTraceAttack 207
#define LUA_POOLEDSTRING_OnLandOnGround 208
#define LUA_POOLEDSTRING_OnLeaveGround 209
#define LUA_POOLEDSTRING_OnNavAreaChanged 210
#define LUA_POOLEDSTRING_OnOtherKilled 211
#define LUA_POOLEDSTRING_OnStuck 212
#define LUA_POOLEDSTRING_OnTakeDamage 213
#define LUA_POOLEDSTRING_OnUnStuck 214
#define LUA_POOLEDSTRING_PassesDamageFilter 215
#define LUA_POOLEDSTRING_PassesFilter 216
#define LUA_POOLEDSTRING_PassesTriggerFilters 217
#define LUA_POOLEDSTRING_PhysicsCollide 218
#define LUA_POOLEDSTRING_PhysicsSimulate 219
#define LUA_POOLEDSTRING_PhysicsUpdate 220
#define LUA_POOLEDSTRING_RunEngineTask 221
#define LUA_POOLEDSTRING_StartEngineSchedule 222
#define LUA_POOLEDSTRING_StartEngineTask 223
#define LUA_POOLEDSTRING_StartTouch 224
#define LUA_POOLEDSTRING_TestCollision 225
#define LUA_POOLEDSTRING_Touch 226
#define LUA_POOLEDSTRING_UpdateTransmitState 227
#define LUA_POOLEDSTRING_Use 228
#define LUA_POOLEDSTRING_IsJumpLegal 229
#define LUA_POOLEDSTRING_CalcWeaponProficiency 230
#define LUA_POOLEDSTRING_GetShadowCastDirection 231
#define LUA_POOLEDSTRING_OnEntitySight 232
#define LUA_POOLEDSTRING_OnEntitySightLost 233
#define LUA_POOLEDSTRING_OnMovementFailed 234
#define LUA_POOLEDSTRING_OnMovementComplete 235
#define LUA_POOLEDSTRING_OnActiveWeaponChanged 236
#define LUA_POOLEDSTRING_RunAI 237
#define LUA_POOLEDSTRING_OnTaskFailed 238
#define LUA_POOLEDSTRING_OverrideMove 239
#define LUA_POOLEDSTRING_OverrideMoveFacing 240
#define LUA_POOLEDSTRING_TranslateSchedule 241
#define LUA_POOLEDSTRING_OnStateChange 242
#define LUA_POOLEDSTRING_CustomAmmoDisplay 243
#define LUA_POOLEDSTRING_DrawHUD 244
#define LUA_POOLEDSTRING_DrawHUDBackground 245
#define LUA_POOLEDSTRING_DrawWorldModel 246
#define LUA_POOLEDSTRING_DrawWorldModelTranslucent 247
#define LUA_POOLEDSTRING_DrawWeaponSelection 248
#define LUA_POOLEDSTRING_DoDrawCrosshair 249
#define LUA_POOLEDSTRING_ShouldDrawViewModel 250
#define LUA_POOLEDSTRING_GetTracerOrigin 251
#define LUA_POOLEDSTRING_OnDrop 252
#define LUA_POOLEDSTRING_OnRemove 253
#define LUA_POOLEDSTRING_PrimaryAttack 254
#define LUA_POOLEDSTRING_SecondaryAttack 255
#define LUA_POOLEDSTRING_Reload 256
#define LUA_POOLEDSTRING_RenderScreen 257
#define LUA_POOLEDSTRING_ViewModelDrawn 258
#define LUA_POOLEDSTRING_TranslateFOV 259
#define LUA_POOLEDSTRING_SetWeaponHoldType 260
#define LUA_POOLEDSTRING_CanBePickedUpByNPCs 261
#define LUA_POOLEDSTRING_NPCShoot_Primary 262
#define LUA_POOLEDSTRING_NPCShoot_Secondary 263
#define LUA_POOLEDSTRING_GetNPCRestTimes 264
#define LUA_POOLEDSTRING_GetNPCBurstSettings 265
#define LUA_POOLEDSTRING_GetNPCBulletSpread 266
#define LUA_POOLEDSTRING_ShouldDropOnDie 267
#define LUA_POOLEDSTRING_ActionSignal 268
#define LUA_POOLEDSTRING_AllowInput 269
#define LUA_POOLEDSTRING_ApplySchemeSettings 270
#define LUA_POOLEDSTRING_ConsoleMessage 271
#define LUA_POOLEDSTRING_DoClick 272
#define LUA_POOLEDSTRING_OnActivate 273
#define LUA_POOLEDSTRING_OnBeginLoadingDocument 274
#define LUA_POOLEDSTRING_OnCallback 275
#define LUA_POOLEDSTRING_OnChangeAddressBar 276
#define LUA_POOLEDSTRING_OnChangeTargetURL 277
#define LUA_POOLEDSTRING_OnChangeTitle 278
#define LUA_POOLEDSTRING_OnChildViewCreated 279
#define LUA_POOLEDSTRING_OnCursorEntered 280
#define LUA_POOLEDSTRING_OnCursorExited 281
#define LUA_POOLEDSTRING_OnCursorMoved 282
#define LUA_POOLEDSTRING_OnDeactivate 283
#define LUA_POOLEDSTRING_OnDeletion 284
#define LUA_POOLEDSTRING_OnDocumentReady 285
#define LUA_POOLEDSTRING_OnFinishLoadingDocument 286
#define LUA_POOLEDSTRING_OnFocusChanged 287
#define LUA_POOLEDSTRING_OnGetFocus 288
#define LUA_POOLEDSTRING_OnKeyCodePressed 289
#define LUA_POOLEDSTRING_OnKeyCodeReleased 290
#define LUA_POOLEDSTRING_OnKeyCodeTyped 291
#define LUA_POOLEDSTRING_OnLoseFocus 292
#define LUA_POOLEDSTRING_OnMousePressed 293
#define LUA_POOLEDSTRING_OnMouseReleased 294
#define LUA_POOLEDSTRING_OnMouseWheeled 295
#define LUA_POOLEDSTRING_OnScreenSizeChanged 296
#define LUA_POOLEDSTRING_OnSizeChanged 297
#define LUA_POOLEDSTRING_OnTextChanged 298
#define LUA_POOLEDSTRING_OnTextClicked 299
#define LUA_POOLEDSTRING_PerformLayout 300
#define LUA_POOLEDSTRING_ResourceLoaded 301
#define LUA_POOLEDSTRING_TestHover 302
#define LUA_POOLEDSTRING_DTVar_ReceiveProxyGL 303
#define LUA_POOLEDSTRING_Float 304
#define LUA_POOLEDSTRING_Int 305
#define LUA_POOLEDSTRING_Bool 306
#define LUA_POOLEDSTRING_Vector 307
#define LUA_POOLEDSTRING_Angle 308
#define LUA_POOLEDSTRING_String 309
#define LUA_POOLEDSTRING_Entity 310