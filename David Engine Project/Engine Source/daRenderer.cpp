#include "daRenderer.h"
#include "daResources.h"
#include "daMesh.h"
#include "daTexture.h"
#include "daShader.h"
#include "daMaterial.h"

using namespace da;
using namespace da::graphics;
using namespace da::math;

namespace renderer
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerStates[(UINT)eRSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilStates[(UINT)eDSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> BlendStates[(UINT)eBSType::End] = {};

	ConstantBuffer* constantBuffer[(UINT)eCBType::End] = {};

	std::vector<Camera*> cameras = {};
	std::vector<DebugMesh> debugMeshs = {};
	
	Camera* mainCamera = nullptr;
	Camera* uiCamera = nullptr;
	GridScript* gridScript = nullptr;

	void LoadMesh()
	{
		std::vector<Vertex> vertexes = {};
		std::vector<UINT> indexes = {};

#pragma region Rect Mesh
		vertexes.resize(4);
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"RectMesh", mesh);
		mesh->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());

		indexes.push_back(0); indexes.push_back(1); indexes.push_back(2);
		indexes.push_back(0); indexes.push_back(2); indexes.push_back(3);

		mesh->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());
#pragma endregion
#pragma region Debug Mesh (rect, circle)
		// Rect Debug Mesh
		std::shared_ptr<Mesh> rectDebug = std::make_shared<Mesh>();
		Resources::Insert(L"DebugRect", rectDebug);
		rectDebug->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());
		rectDebug->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());

		// Circle Debug Mesh
		vertexes.clear();
		indexes.clear();

		Vertex center = {};
		center.pos = Vector3(0.0f, 0.0f, 0.0f);
		center.color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes.push_back(center);

		int iSlice = 40;
		float fRadius = 0.5f;
		float fTheta = XM_2PI / (float)iSlice;

		for (int i = 0; i < iSlice; ++i)
		{
			center.pos = Vector3(fRadius * cosf(fTheta * (float)i)
				, fRadius * sinf(fTheta * (float)i)
				, 0.0f);
			center.color = Vector4(0.0f, 1.0f, 0.0f, 1.f);
			vertexes.push_back(center);
		}

		for (int i = 0; i < vertexes.size() - 2; ++i)
		{
			indexes.push_back(i + 1);
		}
		indexes.push_back(1);

		std::shared_ptr<Mesh> circleDebug = std::make_shared<Mesh>();
		Resources::Insert(L"DebugCircle", circleDebug);
		circleDebug->CreateVertexBuffer(vertexes.data(), (UINT)vertexes.size());
		circleDebug->CreateIndexBuffer(indexes.data(), (UINT)indexes.size());
#pragma endregion
	}
	void LoadBuffer()
	{
#pragma region Create constant buffer 

		// �߰��� cbuffer ���
		constantBuffer[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffer[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		// background
		constantBuffer[(UINT)eCBType::Time] = new ConstantBuffer(eCBType::Time);
		constantBuffer[(UINT)eCBType::Time]->Create(sizeof(TimeCB));

		// Grid
		constantBuffer[(UINT)eCBType::Camera] = new ConstantBuffer(eCBType::Camera);
		constantBuffer[(UINT)eCBType::Camera]->Create(sizeof(CameraCB));

		// BarValue
		constantBuffer[(UINT)eCBType::Life] = new ConstantBuffer(eCBType::Life);
		constantBuffer[(UINT)eCBType::Life]->Create(sizeof(LifeCB));
		constantBuffer[(UINT)eCBType::Count] = new ConstantBuffer(eCBType::Count);
		constantBuffer[(UINT)eCBType::Count]->Create(sizeof(CountCB));

		// fade
		
		// afterimage
		
		// blink

#pragma endregion
	}
	void LoadResource()
	{
#pragma region Create Shaders

		// Default Shaders
		std::shared_ptr<Shader> triangleShader = std::make_shared<Shader>();
		{
			triangleShader->Create(eShaderStage::VS, L"TriangleShader.hlsl", "mainVS");
			triangleShader->Create(eShaderStage::PS, L"TriangleShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"TriangleShader", triangleShader);
		}
		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		{
			spriteShader->Create(eShaderStage::VS, L"SpriteShader.hlsl", "mainVS");
			spriteShader->Create(eShaderStage::PS, L"SpriteShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"SpriteShader", spriteShader);
		}
		// TitleBackground Shader
		std::shared_ptr<Shader> movingBGShader = std::make_shared<Shader>();
		{
			movingBGShader->Create(eShaderStage::VS, L"MovingBGShader.hlsl", "mainVS");
			movingBGShader->Create(eShaderStage::PS, L"MovingBGShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"MovingBGShader", movingBGShader);
		}
		// Grid Shader
		std::shared_ptr<Shader> gridShader = std::make_shared<Shader>();
		{
			gridShader->Create(eShaderStage::VS, L"GridShader.hlsl", "mainVS");
			gridShader->Create(eShaderStage::PS, L"GridShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"GridShader", gridShader);
		}
		std::shared_ptr<Shader> lifeBarShader = std::make_shared<Shader>();
		{
			lifeBarShader->Create(eShaderStage::VS, L"LifeBarShader.hlsl", "mainVS");
			lifeBarShader->Create(eShaderStage::PS, L"LifeBarShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"LifeBarShader", lifeBarShader);
		}
		std::shared_ptr<Shader> dashCountShader = std::make_shared<Shader>();
		{
			dashCountShader->Create(eShaderStage::VS, L"DashCountShader.hlsl", "mainVS");
			dashCountShader->Create(eShaderStage::PS, L"DashCountShader.hlsl", "mainPS");
			Resources::Insert<Shader>(L"DashCountShader", dashCountShader);
		}
		std::shared_ptr<Shader> debugShader = std::make_shared<Shader>();
		{
			debugShader->Create(eShaderStage::VS, L"TriangleShader.hlsl", "mainVS");
			debugShader->Create(eShaderStage::PS, L"DebugShader.hlsl", "mainPS");
			debugShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
			debugShader->SetRatserizerState(eRSType::SolidNone);
			Resources::Insert(L"DebugShader", debugShader);
		}
		std::shared_ptr<Shader> tilingLayerShader = std::make_shared<Shader>();
		{
			tilingLayerShader->Create(eShaderStage::VS, L"TilingLayerShader.hlsl", "mainVS");
			tilingLayerShader->Create(eShaderStage::PS, L"TilingLayerShader.hlsl", "mainPS");
			Resources::Insert(L"TilingLayerShader", tilingLayerShader);
		}
#pragma endregion

#pragma region Sample Material
		// Basic Cursor
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"SampleTexture", L"..\\Resources\\Texture\\Sample\\smileTexture.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"SampleMaterial", spriteMaterial);
		}
#pragma endregion
#pragma region Debug Material
		// rect
		{			
			std::shared_ptr<Material> debugMaterial = std::make_shared<Material>();
			debugMaterial->SetShader(debugShader);
			Resources::Insert<Material>(L"DebugMaterial", debugMaterial);
		}
#pragma endregion
#pragma region HUD Material

		// Grid
		{
			std::shared_ptr<Material> gridMaterial = std::make_shared<Material>();
			gridMaterial->SetShader(gridShader);
			Resources::Insert<Material>(L"GridMaterial", gridMaterial);
		}
		// Basic Cursor
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BasicCursorTexture", L"..\\Resources\\Texture\\UIs\\Hud\\Mouse\\BasicCursor.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"BasicCursorMaterial", spriteMaterial);
		}
		// Shooting Cursor
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"ShootingCursorTexture", L"..\\Resources\\Texture\\UIs\\Hud\\Mouse\\ShootingCursor2.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"ShootingCursorMaterial", spriteMaterial);
		}

		// life Panel
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerLifePanelTexture", L"..\\Resources\\Texture\\UIs\\Hud\\Life\\PlayerLifePanel.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"PlayerLifePanelMaterial", spriteMaterial);
		}
		// life Bar
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"PlayerLifeBarTexture", L"..\\Resources\\Texture\\UIs\\Hud\\Life\\LifeBar.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(lifeBarShader);
			Resources::Insert<Material>(L"PlayerLifeBarMaterial", spriteMaterial);
		}
		// dashcount panel
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DashPanelTexture", L"..\\Resources\\Texture\\UIs\\Hud\\DashCount\\DashPanel.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"DashPanelMaterial", spriteMaterial);
		}
		// dashcount bar
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"DashActivateTexture", L"..\\Resources\\Texture\\UIs\\Hud\\DashCount\\DashCountsActivate.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(dashCountShader);
			Resources::Insert<Material>(L"DashActivateMaterial", spriteMaterial);
		}
		// weapon panel
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"WeaponBaseTexture", L"..\\Resources\\Texture\\UIs\\Hud\\Weapon\\Base.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"WeaponBaseMaterial", spriteMaterial);
		}
#pragma endregion
#pragma region Inventory Material & Texture
		// InventorySelect : NoneTexture Material | Sprite Shader
		{
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"InventoryPanelMaterial", spriteMaterial);
		}
		// WeaponSlot 1~2 : NoneTexture Material | Sprite Shader
		{
			std::shared_ptr<Material> slot1Material = std::make_shared<Material>();
			slot1Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"WeaponSlot1Material", slot1Material);
			std::shared_ptr<Material> slot2Material = std::make_shared<Material>();
			slot2Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"WeaponSlot2Material", slot2Material);
		}
		// shiledSlot 1~2 : NoneTexture Material | Sprite Shader
		{
			std::shared_ptr<Material> slot1Material = std::make_shared<Material>();
			slot1Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ShiledSlot1Material", slot1Material);
			std::shared_ptr<Material> slot2Material = std::make_shared<Material>();
			slot2Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ShiledSlot2Material", slot2Material);
		}
		// accessorySlot 1~4 : NoneTexture Material | Sprite Shader
		{
			std::shared_ptr<Material> slot1Material = std::make_shared<Material>(); 
			slot1Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"AccessorySlot1Material", slot1Material);
			std::shared_ptr<Material> slot2Material = std::make_shared<Material>();
			slot2Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"AccessorySlot2Material", slot2Material);
			std::shared_ptr<Material> slot3Material = std::make_shared<Material>();
			slot3Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"AccessorySlot3Material", slot3Material);
			std::shared_ptr<Material> slot4Material = std::make_shared<Material>();
			slot4Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"AccessorySlot4Material", slot4Material);
		}
		// ItemSlot 1~ 15 : NoneTexture Material | Sprite Shader
		{
			std::shared_ptr<Material> slot00Material = std::make_shared<Material>();
			slot00Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot00Material", slot00Material);
			std::shared_ptr<Material> slot01Material = std::make_shared<Material>();
			slot01Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot01Material", slot01Material);
			std::shared_ptr<Material> slot02Material = std::make_shared<Material>();
			slot02Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot02Material", slot02Material);
			std::shared_ptr<Material> slot03Material = std::make_shared<Material>();
			slot03Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot03Material", slot03Material);
			std::shared_ptr<Material> slot04Material = std::make_shared<Material>();
			slot04Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot04Material", slot04Material);
			std::shared_ptr<Material> slot10Material = std::make_shared<Material>();
			slot10Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot10Material", slot10Material);
			std::shared_ptr<Material> slot11Material = std::make_shared<Material>();
			slot11Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot11Material", slot11Material);
			std::shared_ptr<Material> slot12Material = std::make_shared<Material>();
			slot12Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot12Material", slot12Material);
			std::shared_ptr<Material> slot13Material = std::make_shared<Material>();
			slot13Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot13Material", slot13Material);
			std::shared_ptr<Material> slot14Material = std::make_shared<Material>();
			slot14Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot14Material", slot14Material);
			std::shared_ptr<Material> slot20Material = std::make_shared<Material>();
			slot20Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot20Material", slot20Material);
			std::shared_ptr<Material> slot21Material = std::make_shared<Material>();
			slot21Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot21Material", slot21Material);
			std::shared_ptr<Material> slot22Material = std::make_shared<Material>();
			slot22Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot22Material", slot22Material);
			std::shared_ptr<Material> slot23Material = std::make_shared<Material>();
			slot23Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot23Material", slot23Material);
			std::shared_ptr<Material> slot24Material = std::make_shared<Material>();
			slot24Material->SetShader(spriteShader);
			Resources::Insert<Material>(L"ItemSlot24Material", slot24Material);			
		}

		// Inventory Textures
		{
			Resources::Load<Texture>(L"InventoryPanelATexture", L"..\\Resources\\Texture\\UIs\\Overlay\\Inventory\\InventoryPanelA.png");
			Resources::Load<Texture>(L"InventoryPanelBTexture", L"..\\Resources\\Texture\\UIs\\Overlay\\Inventory\\InventoryPanelB.png");

		}
		// Select Weapon & Shiled Textures
		{
			Resources::Load<Texture>(L"WeaponSlotTexture", L"..\\Resources\\Texture\\UIs\\Overlay\\Inventory\\weapon.png");
			Resources::Load<Texture>(L"WeaponSlotSelectTexture", L"..\\Resources\\Texture\\UIs\\Overlay\\Inventory\\weaponSelect.png");
			Resources::Load<Texture>(L"ShiledSlotTexture", L"..\\Resources\\Texture\\UIs\\Overlay\\Inventory\\shiled.png");
			Resources::Load<Texture>(L"ShiledSlotSelectTexture", L"..\\Resources\\Texture\\UIs\\Overlay\\Inventory\\shiledSelect.png");
		}
		// accessory & item slot
		{
			Resources::Load<Texture>(L"AccessorySlotTexture", L"..\\Resources\\Texture\\UIs\\Overlay\\Inventory\\accessory.png");
			Resources::Load<Texture>(L"AccessorySlotSelectTexture", L"..\\Resources\\Texture\\UIs\\Overlay\\Inventory\\accessorySelect.png");
			Resources::Load<Texture>(L"ItemSlotTexture", L"..\\Resources\\Texture\\UIs\\Overlay\\Inventory\\ItemSlot.png");
			Resources::Load<Texture>(L"ItemSlotSelectTexture", L"..\\Resources\\Texture\\UIs\\Overlay\\Inventory\\ItemSlotSelect.png");
		}
#pragma endregion

#pragma region Title :: Load Texture & Create Material

		// TitleBtn : NoneTexture Material | Sprite Shader
		{
			std::shared_ptr<Material> startBtnMaterial = std::make_shared<Material>();
			startBtnMaterial->SetShader(spriteShader);
			std::shared_ptr<Material> exitBtnMaterial = std::make_shared<Material>();
			exitBtnMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"StartBtnMaterial", startBtnMaterial);
			Resources::Insert<Material>(L"ExitBtnMaterial", exitBtnMaterial);
		}

		// TitleScene BG layer | Sprite Shader	
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TitleBackgroundTexture", L"..\\Resources\\Texture\\Scene_Title\\TitleBackground.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"TitleBackgroundMaterial", spriteMaterial);
		}
		// Front Layer, Back Layer, Small Clouds | MovingBG Shader
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"BackCloudTexture", L"..\\Resources\\Texture\\Scene_Title\\BackCloud.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(movingBGShader);
			Resources::Insert<Material>(L"BackCloudMaterial", spriteMaterial);
		}
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"FrontCloudTexture", L"..\\Resources\\Texture\\Scene_Title\\FrontCloud.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(movingBGShader);
			Resources::Insert<Material>(L"FrontCloudMaterial", spriteMaterial);
		}
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"SmallCloudTexture", L"..\\Resources\\Texture\\Scene_Title\\smallCloud.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(movingBGShader);
			Resources::Insert<Material>(L"SmallCloudMaterial", spriteMaterial);
		}

		// Title MainLogo & CopyRight | sprite shader
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"MainLogoTexture", L"..\\Resources\\Texture\\Scene_Title\\MainLogo.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"MainLogoMaterial", spriteMaterial);
		}
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"CopyrightTexture", L"..\\Resources\\Texture\\Scene_Title\\Copyright.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"CopyrightMaterial", spriteMaterial);
		}

		// TitleScene Button Textures
		{
			Resources::Load<Texture>(L"PlayOffTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\PlayOff_Kor.png");
			Resources::Load<Texture>(L"PlayOnTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\PlayOn_Kor.png");
			
			Resources::Load<Texture>(L"ExitOffTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\ExitOff_Kor.png");
			Resources::Load<Texture>(L"ExitOnTexture", L"..\\Resources\\Texture\\Scene_Title\\Button\\ExitOn_Kor.png");
		}
#pragma endregion
#pragma region Town :: Load Texture & Create material
		// sky
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"SkyTexture", L"..\\Resources\\Texture\\Scene_Town\\layer\\Sky_Day.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			Resources::Insert<Material>(L"SkyMaterial", spriteMaterial);
		}
		// TownBG
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TownBGTexture", L"..\\Resources\\Texture\\Scene_Town\\layer\\TownBG_Day.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(tilingLayerShader);
			Resources::Insert<Material>(L"TownBGMaterial", spriteMaterial);
		}
		// TownLayer
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TownLayerDayTexture", L"..\\Resources\\Texture\\Scene_Town\\layer\\TownLayer_Day.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(tilingLayerShader);
			Resources::Insert<Material>(L"TownLayerMaterial", spriteMaterial);
		}
#pragma endregion
#pragma region 1F :: Load Texture & Create material
		// Temp dungeon
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"TempDungeonTexture", L"..\\Resources\\Texture\\Scene_Dungeon1F\\tempDungeon.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			//spriteMaterial->SetRenderingMode(eRenderingMode::Opaque);
			Resources::Insert<Material>(L"TempDungeonMaterial", spriteMaterial);
		}

#pragma endregion
#pragma region 2F :: Load Texture & Create material
		// Temp dungeon
		{
			std::shared_ptr<Texture> texture
				= Resources::Load<Texture>(L"2FTexture", L"..\\Resources\\Texture\\Scene_Dungeon2F\\2F.png");
			std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
			spriteMaterial->SetTexture(texture);
			spriteMaterial->SetShader(spriteShader);
			//spriteMaterial->SetRenderingMode(eRenderingMode::Opaque);
			Resources::Insert<Material>(L"2FMaterial", spriteMaterial);
		}

#pragma endregion
	}
	void SetupState()
	{
#pragma region InputLayout
		const int numElement = 3;
		D3D11_INPUT_ELEMENT_DESC arrLayout[numElement] = {};

		arrLayout[0].AlignedByteOffset = 0;
		arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[0].InputSlot = 0;
		arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[0].SemanticName = "POSITION";
		arrLayout[0].SemanticIndex = 0;

		arrLayout[1].AlignedByteOffset = 12;
		arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayout[1].InputSlot = 0;
		arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[1].SemanticName = "COLOR";
		arrLayout[1].SemanticIndex = 0;

		arrLayout[2].AlignedByteOffset = 28;
		arrLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayout[2].InputSlot = 0;
		arrLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[2].SemanticName = "TEXCOORD";
		arrLayout[2].SemanticIndex = 0;

		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"TriangleShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());
		
		shader = Resources::Find<Shader>(L"SpriteShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"MovingBGShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"GridShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"LifeBarShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());
		shader = Resources::Find<Shader>(L"DashCountShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"DebugShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

		shader = Resources::Find<Shader>(L"TilingLayerShader");
		GetDevice()->CreateInputLayout(arrLayout, numElement
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());

#pragma endregion
#pragma region Sampler State

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
		GetDevice()->BindSamplers(eShaderStage::PS, 0, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
		
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf()); 
		GetDevice()->BindSamplers(eShaderStage::PS, 1, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf());

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSamplers(eShaderStage::PS, 2, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
#pragma endregion
#pragma region Rasterizer State
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		// Soild Back	: �޸��� ���������� ����
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());
		// Soild Front	: �ո��� ���������� ����
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());
		// Solid None	: ��� ���� ��������
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());
		// Wireframe	: ���븸 ��������
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		GetDevice()->CreateRasterizerState(
			&rasterizerDesc, RasterizerStates[(UINT)eRSType::Wireframe].GetAddressOf());
#pragma endregion
#pragma region DepthStencil State
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		// Less <- Z ���� ������ ������ �ȱ׸�
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(
			&depthStencilDesc, DepthStencilStates[(UINT)eDSType::Less].GetAddressOf());
		// Greater <- Z ���� ������ ũ�� �ȱ׸�
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		GetDevice()->CreateDepthStencilState(
			&depthStencilDesc, DepthStencilStates[(UINT)eDSType::Greater].GetAddressOf());
		// No Write <- ����� ����
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		GetDevice()->CreateDepthStencilState(
			&depthStencilDesc, DepthStencilStates[(UINT)eDSType::NoWrite].GetAddressOf());
		// None 
		depthStencilDesc.DepthEnable = false;
		GetDevice()->CreateDepthStencilState(
			&depthStencilDesc, DepthStencilStates[(UINT)eDSType::None].GetAddressOf());
#pragma endregion
#pragma region Blend State
		D3D11_BLEND_DESC blendDesc = {};
		// Default
		BlendStates[(UINT)eBSType::Default] = nullptr;
		// Alpha Blend
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(
			&blendDesc, BlendStates[(UINT)eBSType::AlphaBlend].GetAddressOf());
		// Oneone
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		GetDevice()->CreateBlendState(
			&blendDesc, BlendStates[(UINT)eBSType::Oneone].GetAddressOf());
#pragma endregion
	}
	void Initialize()
	{
		LoadMesh();
		LoadBuffer();
		LoadResource();
		SetupState();
	}
	void Render()
	{
		for (Camera* camera : cameras)
		{
			if (nullptr == camera)
				continue;

			camera->Render();
		}

		cameras.clear();
	}
	void PushDebugMeshAttribute(DebugMesh mesh)
	{
		debugMeshs.push_back(mesh);
	}

	void Release()
	{
		for (ConstantBuffer* buff : constantBuffer)
		{
			if (buff == nullptr)
				continue;

			delete buff;
			buff = nullptr;
		}
	}
}