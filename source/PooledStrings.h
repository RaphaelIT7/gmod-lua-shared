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
#define LUA_POOLEDSTRING_AcceptInput 1
#define LUA_POOLEDSTRING_AdjustMouseSensitivity 2
#define LUA_POOLEDSTRING_AllowPlayerPickup 3
#define LUA_POOLEDSTRING_CalcMainActivity 4
#define LUA_POOLEDSTRING_CalcView 5
#define LUA_POOLEDSTRING_CalcViewModelView 6
#define LUA_POOLEDSTRING_CanExitVehicle 7
#define LUA_POOLEDSTRING_CanPlayerEnterVehicle 8
#define LUA_POOLEDSTRING_CanPlayerSuicide 9
#define LUA_POOLEDSTRING_ChatText 10
#define LUA_POOLEDSTRING_ChatTextChanged 11
#define LUA_POOLEDSTRING_CheckPassword 12
#define LUA_POOLEDSTRING_ClientSignOnStateChanged 13
#define LUA_POOLEDSTRING_CreateClientsideRagdoll 14
#define LUA_POOLEDSTRING_CreateEntityRagdoll 15
#define LUA_POOLEDSTRING_CreateMove 16
#define LUA_POOLEDSTRING_DoAnimationEvent 17
#define LUA_POOLEDSTRING_DoPlayerDeath 18
#define LUA_POOLEDSTRING_DrawMonitors 19
#define LUA_POOLEDSTRING_DrawPhysgunBeam 20
#define LUA_POOLEDSTRING_EndEntityDriving 21
#define LUA_POOLEDSTRING_EntityEmitSound 22
#define LUA_POOLEDSTRING_EntityFireBullets 23
#define LUA_POOLEDSTRING_PostEntityFireBullets 24
#define LUA_POOLEDSTRING_EntityKeyValue 25
#define LUA_POOLEDSTRING_EntityNetworkedVarChanged 26
#define LUA_POOLEDSTRING_EntityRemoved 27
#define LUA_POOLEDSTRING_EntityTakeDamage 28
#define LUA_POOLEDSTRING_PostEntityTakeDamage 29
#define LUA_POOLEDSTRING_FindUseEntity 30
#define LUA_POOLEDSTRING_FinishChat 31
#define LUA_POOLEDSTRING_FinishMove 32
#define LUA_POOLEDSTRING_GUIMouseDoublePressed 33
#define LUA_POOLEDSTRING_GUIMousePressed 34
#define LUA_POOLEDSTRING_GUIMouseReleased 35
#define LUA_POOLEDSTRING_GetFallDamage 36
#define LUA_POOLEDSTRING_GetGameDescription 37
#define LUA_POOLEDSTRING_GetMotionBlurValues 38
#define LUA_POOLEDSTRING_GetPreferredCarryAngles 39
#define LUA_POOLEDSTRING_GetTeamColor 40
#define LUA_POOLEDSTRING_GravGunOnDropped 41
#define LUA_POOLEDSTRING_GravGunOnPickedUp 42
#define LUA_POOLEDSTRING_GravGunPickupAllowed 43
#define LUA_POOLEDSTRING_GravGunPunt 44
#define LUA_POOLEDSTRING_HUDAmmoPickedUp 45
#define LUA_POOLEDSTRING_HUDDrawScoreBoard 46
#define LUA_POOLEDSTRING_HUDItemPickedUp 47
#define LUA_POOLEDSTRING_HUDPaint 48
#define LUA_POOLEDSTRING_HUDPaintBackground 49
#define LUA_POOLEDSTRING_HUDShouldDraw 50
#define LUA_POOLEDSTRING_HUDWeaponPickedUp 51
#define LUA_POOLEDSTRING_InitPostEntity 52
#define LUA_POOLEDSTRING_InputMouseApply 53
#define LUA_POOLEDSTRING_KeyPress 54
#define LUA_POOLEDSTRING_KeyRelease 55
#define LUA_POOLEDSTRING_Move 56
#define LUA_POOLEDSTRING_NeedsDepthPass 57
#define LUA_POOLEDSTRING_NetworkEntityCreated 58
#define LUA_POOLEDSTRING_NetworkIDValidated 59
#define LUA_POOLEDSTRING_NotifyShouldTransmit 60
#define LUA_POOLEDSTRING_OnAchievementAchieved 61
#define LUA_POOLEDSTRING_OnChatTab 62
#define LUA_POOLEDSTRING_OnCrazyPhysics 63
#define LUA_POOLEDSTRING_OnDamagedByExplosion 64
#define LUA_POOLEDSTRING_OnEntityCreated 65
#define LUA_POOLEDSTRING_OnEntityWaterLevelChanged 66
#define LUA_POOLEDSTRING_OnNPCKilled 67
#define LUA_POOLEDSTRING_OnPhysgunFreeze 68
#define LUA_POOLEDSTRING_OnPhysgunReload 69
#define LUA_POOLEDSTRING_OnPlayerChat 70
#define LUA_POOLEDSTRING_OnPlayerHitGround 71
#define LUA_POOLEDSTRING_PlayerChangedTeam 72
#define LUA_POOLEDSTRING_OnViewModelChanged 73
#define LUA_POOLEDSTRING_PhysgunDrop 74
#define LUA_POOLEDSTRING_PhysgunPickup 75
#define LUA_POOLEDSTRING_OnPhysgunPickup 76
#define LUA_POOLEDSTRING_PlayerAuthed 77
#define LUA_POOLEDSTRING_PlayerBindPress 78
#define LUA_POOLEDSTRING_PlayerButtonDown 79
#define LUA_POOLEDSTRING_PlayerButtonUp 80
#define LUA_POOLEDSTRING_PlayerCanHearPlayersVoice 81
#define LUA_POOLEDSTRING_PlayerCanPickupItem 82
#define LUA_POOLEDSTRING_PlayerCanPickupWeapon 83
#define LUA_POOLEDSTRING_PlayerCanSeePlayersChat 84
#define LUA_POOLEDSTRING_PlayerConnect 85
#define LUA_POOLEDSTRING_PlayerDeath 86
#define LUA_POOLEDSTRING_PlayerDeathSound 87
#define LUA_POOLEDSTRING_PlayerDeathThink 88
#define LUA_POOLEDSTRING_PlayerDroppedWeapon 89
#define LUA_POOLEDSTRING_PlayerDisconnected 90
#define LUA_POOLEDSTRING_PlayerDriveAnimate 91
#define LUA_POOLEDSTRING_PlayerEndVoice 92
#define LUA_POOLEDSTRING_PlayerEnteredVehicle 93
#define LUA_POOLEDSTRING_PlayerFootstep 94
#define LUA_POOLEDSTRING_PlayerHurt 95
#define LUA_POOLEDSTRING_PlayerInitialSpawn 96
#define LUA_POOLEDSTRING_PlayerLeaveVehicle 97
#define LUA_POOLEDSTRING_PlayerNoClip 98
#define LUA_POOLEDSTRING_PlayerPostThink 99
#define LUA_POOLEDSTRING_PlayerSay 100
#define LUA_POOLEDSTRING_PlayerSelectSpawn 101
#define LUA_POOLEDSTRING_PlayerShouldTakeDamage 102
#define LUA_POOLEDSTRING_PlayerShouldTaunt 103
#define LUA_POOLEDSTRING_PlayerSilentDeath 104
#define LUA_POOLEDSTRING_PlayerSpawn 105
#define LUA_POOLEDSTRING_PlayerSpray 106
#define LUA_POOLEDSTRING_PlayerStartTaunt 107
#define LUA_POOLEDSTRING_PlayerStartVoice 108
#define LUA_POOLEDSTRING_PlayerStepSoundTime 109
#define LUA_POOLEDSTRING_PlayerSwitchFlashlight 110
#define LUA_POOLEDSTRING_PlayerSwitchWeapon 111
#define LUA_POOLEDSTRING_PlayerTick 112
#define LUA_POOLEDSTRING_PlayerTraceAttack 113
#define LUA_POOLEDSTRING_PlayerUse 114
#define LUA_POOLEDSTRING_PostCleanupMap 115
#define LUA_POOLEDSTRING_PostDraw2DSkyBox 116
#define LUA_POOLEDSTRING_PostDrawEffects 117
#define LUA_POOLEDSTRING_PostDrawHUD 118
#define LUA_POOLEDSTRING_PostDrawOpaqueRenderables 119
#define LUA_POOLEDSTRING_PostDrawSkyBox 120
#define LUA_POOLEDSTRING_PostDrawTranslucentRenderables 121
#define LUA_POOLEDSTRING_PostDrawViewModel 122
#define LUA_POOLEDSTRING_PostPlayerDeath 123
#define LUA_POOLEDSTRING_PostPlayerDraw 124
#define LUA_POOLEDSTRING_PostRender 125
#define LUA_POOLEDSTRING_PostRenderVGUI 126
#define LUA_POOLEDSTRING_PreCleanupMap 127
#define LUA_POOLEDSTRING_PreDrawEffects 128
#define LUA_POOLEDSTRING_PreDrawHUD 129
#define LUA_POOLEDSTRING_PreDrawOpaqueRenderables 130
#define LUA_POOLEDSTRING_PreDrawSkyBox 131
#define LUA_POOLEDSTRING_PreDrawTranslucentRenderables 132
#define LUA_POOLEDSTRING_PreDrawViewModel 133
#define LUA_POOLEDSTRING_PreDrawViewModels 134
#define LUA_POOLEDSTRING_PrePlayerDraw 135
#define LUA_POOLEDSTRING_PreRender 136
#define LUA_POOLEDSTRING_PreventScreenClicks 137
#define LUA_POOLEDSTRING_PropBreak 138
#define LUA_POOLEDSTRING_RenderScene 139
#define LUA_POOLEDSTRING_RenderScreenspaceEffects 140
#define LUA_POOLEDSTRING_Restored 141
#define LUA_POOLEDSTRING_Saved 142
#define LUA_POOLEDSTRING_ScaleNPCDamage 143
#define LUA_POOLEDSTRING_ScalePlayerDamage 144
#define LUA_POOLEDSTRING_ScoreboardHide 145
#define LUA_POOLEDSTRING_ScoreboardShow 146
#define LUA_POOLEDSTRING_SetupMove 147
#define LUA_POOLEDSTRING_SetupPlayerVisibility 148
#define LUA_POOLEDSTRING_SetupSkyboxFog 149
#define LUA_POOLEDSTRING_SetupWorldFog 150
#define LUA_POOLEDSTRING_ShouldCollide 151
#define LUA_POOLEDSTRING_ShouldDrawLocalPlayer 152
#define LUA_POOLEDSTRING_ShouldDrawParticles 153
#define LUA_POOLEDSTRING_ShowHelp 154
#define LUA_POOLEDSTRING_ShowSpare1 155
#define LUA_POOLEDSTRING_ShowSpare2 156
#define LUA_POOLEDSTRING_ShowTeam 157
#define LUA_POOLEDSTRING_ShutDown 158
#define LUA_POOLEDSTRING_SpawniconGenerated 159
#define LUA_POOLEDSTRING_StartChat 160
#define LUA_POOLEDSTRING_StartCommand 161
#define LUA_POOLEDSTRING_StartEntityDriving 162
#define LUA_POOLEDSTRING_Think 163
#define LUA_POOLEDSTRING_Tick 164
#define LUA_POOLEDSTRING_TranslateActivity 165
#define LUA_POOLEDSTRING_UpdateAnimation 166
#define LUA_POOLEDSTRING_VGUIMousePressAllowed 167
#define LUA_POOLEDSTRING_VGUIMousePressed 168
#define LUA_POOLEDSTRING_VehicleMove 169
#define LUA_POOLEDSTRING_ViewPunch 170
#define LUA_POOLEDSTRING_WeaponEquip 171
#define LUA_POOLEDSTRING_PlayerAmmoChanged 172
#define LUA_POOLEDSTRING_PlayerClassChanged 173
#define LUA_POOLEDSTRING_PlayerHandleAnimEvent 174
#define LUA_POOLEDSTRING_PlayerFireAnimationEvent 175
#define LUA_POOLEDSTRING_OnPlayerPhysicsPickup 176
#define LUA_POOLEDSTRING_OnPlayerPhysicsDrop 177
#define LUA_POOLEDSTRING_OnPlayerJump 178
#define LUA_POOLEDSTRING_OnCloseCaptionEmit 179
#define LUA_POOLEDSTRING_HandlePlayerArmorReduction 180
#define LUA_POOLEDSTRING_OnChangeActivity 181
#define LUA_POOLEDSTRING_BehaveUpdate 182
#define LUA_POOLEDSTRING_Blocked 183
#define LUA_POOLEDSTRING_BodyUpdate 184
#define LUA_POOLEDSTRING_Deploy 185
#define LUA_POOLEDSTRING_DoImpactEffect 186
#define LUA_POOLEDSTRING_Draw 187
#define LUA_POOLEDSTRING_DrawTranslucent 188
#define LUA_POOLEDSTRING_EndTouch 189
#define LUA_POOLEDSTRING_Equip 190
#define LUA_POOLEDSTRING_EquipAmmo 191
#define LUA_POOLEDSTRING_ExpressionFinished 192
#define LUA_POOLEDSTRING_FireAnimationEvent 193
#define LUA_POOLEDSTRING_FreezeMovement 194
#define LUA_POOLEDSTRING_GetAttackSpread 195
#define LUA_POOLEDSTRING_GetCapabilities 196
#define LUA_POOLEDSTRING_GetRelationship 197
#define LUA_POOLEDSTRING_GetRenderMesh 198
#define LUA_POOLEDSTRING_HandleAnimEvent 199
#define LUA_POOLEDSTRING_Holster 200
#define LUA_POOLEDSTRING_ImpactTrace 201
#define LUA_POOLEDSTRING_KeyValue 202
#define LUA_POOLEDSTRING_OnCondition 203
#define LUA_POOLEDSTRING_OnContact 204
#define LUA_POOLEDSTRING_OnIgnite 205
#define LUA_POOLEDSTRING_OnInjured 206
#define LUA_POOLEDSTRING_OnKilled 207
#define LUA_POOLEDSTRING_OnTraceAttack 208
#define LUA_POOLEDSTRING_OnLandOnGround 209
#define LUA_POOLEDSTRING_OnLeaveGround 210
#define LUA_POOLEDSTRING_OnNavAreaChanged 211
#define LUA_POOLEDSTRING_OnOtherKilled 212
#define LUA_POOLEDSTRING_OnStuck 213
#define LUA_POOLEDSTRING_OnTakeDamage 214
#define LUA_POOLEDSTRING_OnUnStuck 215
#define LUA_POOLEDSTRING_PassesDamageFilter 216
#define LUA_POOLEDSTRING_PassesFilter 217
#define LUA_POOLEDSTRING_PassesTriggerFilters 218
#define LUA_POOLEDSTRING_PhysicsCollide 219
#define LUA_POOLEDSTRING_PhysicsSimulate 220
#define LUA_POOLEDSTRING_PhysicsUpdate 221
#define LUA_POOLEDSTRING_RunEngineTask 222
#define LUA_POOLEDSTRING_StartEngineSchedule 223
#define LUA_POOLEDSTRING_StartEngineTask 224
#define LUA_POOLEDSTRING_StartTouch 225
#define LUA_POOLEDSTRING_TestCollision 226
#define LUA_POOLEDSTRING_Touch 227
#define LUA_POOLEDSTRING_UpdateTransmitState 228
#define LUA_POOLEDSTRING_Use 229
#define LUA_POOLEDSTRING_IsJumpLegal 230
#define LUA_POOLEDSTRING_CalcWeaponProficiency 231
#define LUA_POOLEDSTRING_GetShadowCastDirection 232
#define LUA_POOLEDSTRING_OnEntitySight 233
#define LUA_POOLEDSTRING_OnEntitySightLost 234
#define LUA_POOLEDSTRING_OnMovementFailed 235
#define LUA_POOLEDSTRING_OnMovementComplete 236
#define LUA_POOLEDSTRING_OnActiveWeaponChanged 237
#define LUA_POOLEDSTRING_RunAI 238
#define LUA_POOLEDSTRING_OnTaskFailed 239
#define LUA_POOLEDSTRING_OverrideMove 240
#define LUA_POOLEDSTRING_OverrideMoveFacing 241
#define LUA_POOLEDSTRING_TranslateSchedule 242
#define LUA_POOLEDSTRING_OnStateChange 243
#define LUA_POOLEDSTRING_CustomAmmoDisplay 244
#define LUA_POOLEDSTRING_DrawHUD 245
#define LUA_POOLEDSTRING_DrawHUDBackground 246
#define LUA_POOLEDSTRING_DrawWorldModel 247
#define LUA_POOLEDSTRING_DrawWorldModelTranslucent 248
#define LUA_POOLEDSTRING_DrawWeaponSelection 249
#define LUA_POOLEDSTRING_DoDrawCrosshair 250
#define LUA_POOLEDSTRING_ShouldDrawViewModel 251
#define LUA_POOLEDSTRING_GetTracerOrigin 252
#define LUA_POOLEDSTRING_OnDrop 253
#define LUA_POOLEDSTRING_OnRemove 254
#define LUA_POOLEDSTRING_PrimaryAttack 255
#define LUA_POOLEDSTRING_SecondaryAttack 256
#define LUA_POOLEDSTRING_Reload 257
#define LUA_POOLEDSTRING_RenderScreen 258
#define LUA_POOLEDSTRING_ViewModelDrawn 259
#define LUA_POOLEDSTRING_TranslateFOV 260
#define LUA_POOLEDSTRING_SetWeaponHoldType 261
#define LUA_POOLEDSTRING_CanBePickedUpByNPCs 262
#define LUA_POOLEDSTRING_NPCShoot_Primary 263
#define LUA_POOLEDSTRING_NPCShoot_Secondary 264
#define LUA_POOLEDSTRING_GetNPCRestTimes 265
#define LUA_POOLEDSTRING_GetNPCBurstSettings 266
#define LUA_POOLEDSTRING_GetNPCBulletSpread 267
#define LUA_POOLEDSTRING_ShouldDropOnDie 268
#define LUA_POOLEDSTRING_ActionSignal 269
#define LUA_POOLEDSTRING_AllowInput 270
#define LUA_POOLEDSTRING_ApplySchemeSettings 271
#define LUA_POOLEDSTRING_ConsoleMessage 272
#define LUA_POOLEDSTRING_DoClick 273
#define LUA_POOLEDSTRING_OnActivate 274
#define LUA_POOLEDSTRING_OnBeginLoadingDocument 275
#define LUA_POOLEDSTRING_OnCallback 276
#define LUA_POOLEDSTRING_OnChangeAddressBar 277
#define LUA_POOLEDSTRING_OnChangeTargetURL 278
#define LUA_POOLEDSTRING_OnChangeTitle 279
#define LUA_POOLEDSTRING_OnChildViewCreated 280
#define LUA_POOLEDSTRING_OnCursorEntered 281
#define LUA_POOLEDSTRING_OnCursorExited 282
#define LUA_POOLEDSTRING_OnCursorMoved 283
#define LUA_POOLEDSTRING_OnDeactivate 284
#define LUA_POOLEDSTRING_OnDeletion 285
#define LUA_POOLEDSTRING_OnDocumentReady 286
#define LUA_POOLEDSTRING_OnFinishLoadingDocument 287
#define LUA_POOLEDSTRING_OnFocusChanged 288
#define LUA_POOLEDSTRING_OnGetFocus 289
#define LUA_POOLEDSTRING_OnKeyCodePressed 290
#define LUA_POOLEDSTRING_OnKeyCodeReleased 291
#define LUA_POOLEDSTRING_OnKeyCodeTyped 292
#define LUA_POOLEDSTRING_OnLoseFocus 293
#define LUA_POOLEDSTRING_OnMousePressed 294
#define LUA_POOLEDSTRING_OnMouseReleased 295
#define LUA_POOLEDSTRING_OnMouseWheeled 296
#define LUA_POOLEDSTRING_OnScreenSizeChanged 297
#define LUA_POOLEDSTRING_OnSizeChanged 298
#define LUA_POOLEDSTRING_OnTextChanged 299
#define LUA_POOLEDSTRING_OnTextClicked 300
#define LUA_POOLEDSTRING_PerformLayout 301
#define LUA_POOLEDSTRING_ResourceLoaded 302
#define LUA_POOLEDSTRING_TestHover 303
#define LUA_POOLEDSTRING_DTVar_ReceiveProxyGL 304
#define LUA_POOLEDSTRING_Float 305
#define LUA_POOLEDSTRING_Int 306
#define LUA_POOLEDSTRING_Bool 307
#define LUA_POOLEDSTRING_Vector 308
#define LUA_POOLEDSTRING_Angle 309
#define LUA_POOLEDSTRING_String 310
#define LUA_POOLEDSTRING_Entity 311