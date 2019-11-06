
#include "camera.h"
#include "myDirect3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_V_X		(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_V_Y		(500.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_V_Z		(-500.0f)				// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X		(0.0f)					// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y		(0.0f)					// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z		(0.0f)					// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(1000.0f)										// �r���[���ʂ�FarZ�l

//*****************************************************************************
// �J�����̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
} CAMERA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
CAMERA g_camera;		// �J�������

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �J�����̏���������
//=============================================================================
void Camera_Initialize(void)
{
	g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void Camera_Finalize(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void Camera_Update(void)
{
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void Camera_SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice(); 

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								VIEW_ANGLE,				// �r���[���ʂ̎���p
								VIEW_ASPECT,			// �r���[���ʂ̃A�X�y�N�g��
								VIEW_NEAR_Z,			// �r���[���ʂ�NearZ�l
								VIEW_FAR_Z);			// �r���[���ʂ�FarZ�l

	D3DXMATRIX mtxProj;
	D3DXMatrixIdentity(&mtxProj);

	float aspect = D3DXToRadian(45);//�p�x�����W�A���ɕϊ��H

	float w = 1 / tanf((aspect*1.78)*0.5f);
	float h = 1 / tanf(aspect*0.5f); 
	float Q = VIEW_FAR_Z / (VIEW_FAR_Z - VIEW_NEAR_Z);
	
	mtxProj(0, 0) = w;
	mtxProj(1, 1) = h;
	mtxProj(2, 2) = Q;
	mtxProj(2, 3) = 1.0f;
	mtxProj(3, 2) = Q * VIEW_FAR_Z*-1;



	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView, 
						&g_camera.posV,		// �J�����̎��_
						&g_camera.posR,		// �J�����̒����_
						&g_camera.vecU);	// �J�����̏�����x�N�g��
	
	//��̏����̒��g
	/*D3DXMATRIX mtxView;
	D3DXMatrixIdentity(&mtxView);

	D3DXVECTOR3 axisZ;//3�����̂���
	D3DXVECTOR3 axisX;//3������x��
	D3DXVECTOR3 axisY;//3������y��

	axisZ = g_camera.posR - g_camera.posV;//at-pos�̃x�N�g���v�Z

	D3DXVec3Normalize(&axisZ, &axisZ);//�P�ʃx�N�g����

	//up�x�N�g���Ƃ����ŊO�ς����X�����Z�o
	D3DXVec3Cross(&axisX, &g_camera.vecU, &axisZ);//�O�όv�Z�i������������ϐ�,�v�Z�p�̕ϐ�,�v�Z�p�̕ϐ��j

	//Z����X���̊O�ς��Ƃ�AY�����Z�o
	D3DXVec3Cross(&axisY, &axisZ, &axisX);

	//pos�x�N�g���Ɗe���Ƃ̓��ς����߁��|�P
	float cx = D3DXVec3Dot(&g_camera.posV, &axisX)*-1;
	float cy = D3DXVec3Dot(&g_camera.posV, &axisY)*-1;
	float cz = D3DXVec3Dot(&g_camera.posV, &axisZ)*-1;

	mtxView(0, 0) = axisX.x;//�s��iy,x�j�ɑ��
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
	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

