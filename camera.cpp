
#include "camera.h"
#include "myDirect3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAM_POS_V_X		(0.0f)					// カメラの視点初期位置(X座標)
#define	CAM_POS_V_Y		(500.0f)				// カメラの視点初期位置(Y座標)
#define	CAM_POS_V_Z		(-500.0f)				// カメラの視点初期位置(Z座標)
#define	CAM_POS_R_X		(0.0f)					// カメラの注視点初期位置(X座標)
#define	CAM_POS_R_Y		(0.0f)					// カメラの注視点初期位置(Y座標)
#define	CAM_POS_R_Z		(0.0f)					// カメラの注視点初期位置(Z座標)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(1000.0f)										// ビュー平面のFarZ値

//*****************************************************************************
// カメラの構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 vecU;			// 上方向ベクトル
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
} CAMERA;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
CAMERA g_camera;		// カメラ情報

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// カメラの初期化処理
//=============================================================================
void Camera_Initialize(void)
{
	g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void Camera_Finalize(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void Camera_Update(void)
{
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void Camera_SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice(); 

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								VIEW_ANGLE,				// ビュー平面の視野角
								VIEW_ASPECT,			// ビュー平面のアスペクト比
								VIEW_NEAR_Z,			// ビュー平面のNearZ値
								VIEW_FAR_Z);			// ビュー平面のFarZ値

	D3DXMATRIX mtxProj;
	D3DXMatrixIdentity(&mtxProj);

	float aspect = D3DXToRadian(45);//角度をラジアンに変換？

	float w = 1 / tanf((aspect*1.78)*0.5f);
	float h = 1 / tanf(aspect*0.5f); 
	float Q = VIEW_FAR_Z / (VIEW_FAR_Z - VIEW_NEAR_Z);
	
	mtxProj(0, 0) = w;
	mtxProj(1, 1) = h;
	mtxProj(2, 2) = Q;
	mtxProj(2, 3) = 1.0f;
	mtxProj(3, 2) = Q * VIEW_FAR_Z*-1;



	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView, 
						&g_camera.posV,		// カメラの視点
						&g_camera.posR,		// カメラの注視点
						&g_camera.vecU);	// カメラの上方向ベクトル
	
	//上の処理の中身
	/*D3DXMATRIX mtxView;
	D3DXMatrixIdentity(&mtxView);

	D3DXVECTOR3 axisZ;//3次元のｚ軸
	D3DXVECTOR3 axisX;//3次元のx軸
	D3DXVECTOR3 axisY;//3次元のy軸

	axisZ = g_camera.posR - g_camera.posV;//at-posのベクトル計算

	D3DXVec3Normalize(&axisZ, &axisZ);//単位ベクトル化

	//upベクトルとｚ軸で外積を取りX軸を算出
	D3DXVec3Cross(&axisX, &g_camera.vecU, &axisZ);//外積計算（答えを代入する変数,計算用の変数,計算用の変数）

	//Z軸とX軸の外積をとり、Y軸を算出
	D3DXVec3Cross(&axisY, &axisZ, &axisX);

	//posベクトルと各軸との内積を求め＊－１
	float cx = D3DXVec3Dot(&g_camera.posV, &axisX)*-1;
	float cy = D3DXVec3Dot(&g_camera.posV, &axisY)*-1;
	float cz = D3DXVec3Dot(&g_camera.posV, &axisZ)*-1;

	mtxView(0, 0) = axisX.x;//行列（y,x）に代入
	mtxView(1, 0) = axisX.y;
	mtxView(2, 0) = axisX.z;
	mtxView(0, 1) = axisY.x;
	mtxView(1, 1) = axisY.y;
	mtxView(2, 1) = axisY.z;
	mtxView(0, 2) = axisZ.x;
	mtxView(1, 2) = axisZ.y;
	mtxView(2, 2) = axisZ.z;
	mtxView(3, 0) = cx;
	mtxView(3, 1) = cy;
	mtxView(3, 2) = cz;
	*/
	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

