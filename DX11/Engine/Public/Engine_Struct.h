#pragma once

namespace Engine
{
	enum MOUSEBUTTON { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_X, MBS_END };
	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	typedef struct tagLineIndices32
	{
		unsigned long _1, _2;
	}LINEINDICES32;

	typedef struct tagLineIndices16
	{
		unsigned short _1, _2;
	}LINEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long _1, _2, _3;
	}FACEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short _1, _2, _3;
	}FACEINDICES16;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_SPOTLIGHT,TYPE_END };

		TYPE			eType = TYPE_END;

		XMFLOAT4		vDirection;

		XMFLOAT4		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

	}LIGHTDESC;

	typedef struct tagModelMaterial
	{
		class CTexture* pTextures[AI_TEXTURE_TYPE_MAX];
	}MODEL_MATERIAL;

	typedef struct tagVertex_Model
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagtagVertex_Model_Declaration
	{
		static const unsigned int		iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXMODEL_DECLARATION;

	typedef struct tagVertex_AnimModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex;
		XMFLOAT4		vBlendWeight;
	}VTXANIM;

	typedef struct ENGINE_DLL tagVertex_AnimModel_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXANIM_DECLARATION;


	typedef struct tagVertex_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertex_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEX_DECLARATION;

	typedef struct tagVertex_Point
	{
		XMFLOAT3	vPosition;
		XMFLOAT2	vPSize;
	}VTXPOINT;

	typedef struct tagVertex_Instance
	{
		XMFLOAT4	vRight;
		XMFLOAT4	vUp;
		XMFLOAT4	vLook;
		XMFLOAT4	vTranslation;
	}VTXINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Point_Instance_Declaration
	{
		static const unsigned int iNumElement = 6;
		static const D3D11_INPUT_ELEMENT_DESC Element[iNumElement];
	}VTXPOINT_INSTANCE_DECLARATION;


	typedef struct tagVertex_Normal_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Normal_Texture_Declaration
	{
		static const unsigned int		iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXNORTEX_DECLARATION;

	typedef struct tagVertex_Cube_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexUV;
	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertex_Cube_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXCUBETEX_DECLARATION;

	typedef struct tagVertex_Position
	{
		XMFLOAT3		vPosition;
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Declaration
	{
		static const unsigned int		iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXPOS_DECLARATION;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		unsigned int iWinCX, iWinCY;
		HWND	hWnd;
		WINMODE	isWindowMode;
	}GRAPHICDESC;

	typedef struct tagRay
	{
		
		_float3 vPos;
		_float3 vDir;
		_float fLength;
	}RAY;


	enum class KEY_STATE {
		TAP, // 막 누른 시점
		HOLD,// 꾹 누르고 있음
		AWAY,// 막 뗀 시점
		NONE,// 걍 안눌린 상태
	};
	enum class KEY
	{
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
		NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0,
		F1, F2, F3, F4,
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		ALT,
		LSHIFT,
		SPACE,
		CTRL,
		ENTER,
		ESC,
		TAB,
		LBUTTON,
		RBUTTON,
		BACKSPACE,
		DELETEKEY,
		LAST,
	};

	enum class COLLISION_TYPE//이순서대로 충돌
	{
		DEFAULT = 0,
		PLAYER,
		GHOST,
		GHOST_ATTACK,
		EFFECTS,
		SIGHT,
		DOOR,
		LIGHTSWITCH,
		CAMERA,
		MOUSE,
		OBJECT,
		WALL,
		THERMOMETER,
		ITEM,
		SPIRITBOX,
		TRIPOD,
		ATMOSPHERE,
		GHOST_AREA,
		GHOST_SPAWNPOINT,
		TYPE_END
	};

	union COLLIDER_ID
	{
		struct
		{
			unsigned long Left_id;
			unsigned long Right_id;
		};
		unsigned long long  ID;

	};

	/*For Object Load & Save*/
	enum class LAYER
	{
		PLAYER,
		CAMERA,
		GHOST,
		ITEM,
		OBJECT,
		HOUSE,
		MISC,
		LAYER_END
	};

	enum class OBJ_TAG
	{
		DOTSPROJECTER,
		FLASHLIGHT,
		THERMOMETER,
		EMF,
		NOTE,
		SPIRITBOX,
		VIDEOCAMERA,
		TRAILCAM,
		TRIPOD,
		GHOST,
		ATMOSPHERE,
		DOOR,
		LIGHTSWITCH,
		OBJ_END
	};

	enum class MODEL_TAG
	{
		ABANDONEDMARKET,
		FURNISHEDCABIN,
		GARAGE,
		PIER_HOUSE,
		PIER_HOUSE2,
		ROOFTOP,
		ROOFTOP_BACKGROUND1,
		ROOFTOP_BACKGROUND2,
		ROOFTOP_BACKGROUND3,
		ROOFTOP_BACKGROUND4,
		ROOFTOP_BACKGROUND5,
		ROOFTOP_BACKGROUND6,
		ROOFTOP_BACKGROUND7,
		ROOFTOP_BACKGROUND8,
		ROOFTOP_BACKGROUND9,
		ROOFTOP_BACKGROUND10,
		ROOFTOP_BACKGROUND11,
		ROOFTOP_BACKGROUND12,
		ROOFTOP_BACKGROUND13,
		ROOFTOP_BACKGROUND14,
		SLUMHOUSE1,
		OLDHOUSE,
		SHELTER,
		TRUCK,
		MAINDOOR,
		ROOMDOOR,
		LIGHTSWITCH,
		MODEL_END
	};

	/*저장할때 같이 보내줄 데이터*/
	typedef struct tagObjData
	{
		LAYER		tLayerTag;
		OBJ_TAG		tObjTag;
		_matrix		matWorld;
	} OBJ_DATA;

	typedef struct tagHouseData
	{
		MODEL_TAG	tModelTag;
		_matrix		matWorld;
	}MAP_DATA;//layer 무조건 house

	typedef struct tagCollider
	{
		_matrix matWorld;
	}COLLIDER_DATA;


}