
#include "grid.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	VALUE_MOVE		(5.0f)							// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// ��]��

#define	FIELD_WIDTH		(100.0f)						// �n�ʂ̕�(X����)
#define	FIELD_DEPTH		(100.0f)						// �n�ʂ̉��s(Z����)
#define FIELD_HIGHT     (100.0f)                        //�n�ʂ̍���(Y����)

#define LINE_MAX        (1000)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexGrid(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGrid = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldGrid;		// ���[���h�}�g���b�N�X
D3DXVECTOR3				g_posGrid;				// �n�ʂ̈ʒu
D3DXVECTOR3				g_rotGrid;				// �n�ʂ̌���(��])
D3DXVECTOR3				g_sclGrid;				// �n�ʂ̑傫��(�X�P�[��)

//=============================================================================
// ����������
//=============================================================================
HRESULT Grid_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexGrid(pDevice);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclGrid = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Grid_Finalize(void)
{
	if (g_pVtxBuffGrid != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffGrid->Release();
		g_pVtxBuffGrid = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Grid_Update(void)
{
	if (Keyboard_IsPress(DIK_A))
	{
		if (Keyboard_IsPress(DIK_W))
		{// �����ړ�
			g_posGrid.x += sinf(-D3DX_PI * 0.75f) * VALUE_MOVE;
			g_posGrid.z -= cosf(-D3DX_PI * 0.75f) * VALUE_MOVE;
		}
		else if (Keyboard_IsPress(DIK_S))
		{// ����O�ړ�
			g_posGrid.x += sinf(-D3DX_PI * 0.25f) * VALUE_MOVE;
			g_posGrid.z -= cosf(-D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// ���ړ�
			g_posGrid.x += sinf(-D3DX_PI * 0.50f) * VALUE_MOVE;
			g_posGrid.z -= cosf(-D3DX_PI * 0.50f) * VALUE_MOVE;
		}
	}
	else if (Keyboard_IsPress(DIK_D))
	{
		if (Keyboard_IsPress(DIK_W))
		{// �E���ړ�
			g_posGrid.x += sinf(D3DX_PI * 0.75f) * VALUE_MOVE;
			g_posGrid.z -= cosf(D3DX_PI * 0.75f) * VALUE_MOVE;
		}
		else if (Keyboard_IsPress(DIK_S))
		{// �E��O�ړ�
			g_posGrid.x += sinf(D3DX_PI * 0.25f) * VALUE_MOVE;
			g_posGrid.z -= cosf(D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// �E�ړ�
			g_posGrid.x += sinf(D3DX_PI * 0.50f) * VALUE_MOVE;
			g_posGrid.z -= cosf(D3DX_PI * 0.50f) * VALUE_MOVE;
		}
	}
	else if (Keyboard_IsPress(DIK_W))
	{// ���ړ�
		g_posGrid.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
		g_posGrid.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
	}
	else if (Keyboard_IsPress(DIK_S))
	{// ��O�ړ�
		g_posGrid.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
		g_posGrid.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
	}

#if 1
	if (Keyboard_IsPress(DIK_Q))
	{// Y������]
		g_rotGrid.y -= VALUE_ROTATE;
		if (g_rotGrid.y > D3DX_PI)
		{
			g_rotGrid.y -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.y < -D3DX_PI)
		{
			g_rotGrid.y += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_E))
	{// Y���E��]
		g_rotGrid.y += VALUE_ROTATE;
		if (g_rotGrid.y > D3DX_PI)
		{
			g_rotGrid.y -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.y < -D3DX_PI)
		{
			g_rotGrid.y += D3DX_PI * 2.0f;
		}
	}
#endif

#if 1
	if (Keyboard_IsPress(DIK_UP))
	{// X���E��]
		g_rotGrid.x += VALUE_ROTATE;
		if (g_rotGrid.x > D3DX_PI)
		{
			g_rotGrid.x -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.x < -D3DX_PI)
		{
			g_rotGrid.x += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_DOWN))
	{// X������]
		g_rotGrid.x -= VALUE_ROTATE;
		if (g_rotGrid.x > D3DX_PI)
		{
			g_rotGrid.x -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.x < -D3DX_PI)
		{
			g_rotGrid.x += D3DX_PI * 2.0f;
		}
	}
#endif

#if 1
	if (Keyboard_IsPress(DIK_LEFT))
	{// Z���E��]
		g_rotGrid.z += VALUE_ROTATE;
		if (g_rotGrid.z > D3DX_PI)
		{
			g_rotGrid.z -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.z < -D3DX_PI)
		{
			g_rotGrid.z += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_RIGHT))
	{// Z������]
		g_rotGrid.z -= VALUE_ROTATE;
		if (g_rotGrid.z > D3DX_PI)
		{
			g_rotGrid.z -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.z < -D3DX_PI)
		{
			g_rotGrid.z += D3DX_PI * 2.0f;
		}
	}
#endif

	if (Keyboard_IsPress(DIK_RETURN))
	{// ���Z�b�g
		g_posGrid.x = 0.0f;
		g_posGrid.y = 0.0f;
		g_posGrid.z = 0.0f;

		g_rotGrid.x = 0.0f;
		g_rotGrid.y = 0.0f;
		g_rotGrid.z = 0.0f;
	}

	DebugProc_Print((char*)"*** �R�c�|���S������ ***\n");
	DebugProc_Print((char*)"�ʒu [%f : %f : %f]\n", g_posGrid.x, g_posGrid.y, g_posGrid.z);
	DebugProc_Print((char*)"�O�ړ� : �v\n");
	DebugProc_Print((char*)"��ړ� : �r\n");
	DebugProc_Print((char*)"���ړ� : �`\n");
	DebugProc_Print((char*)"�E�ړ� : �c\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"���� [%f : %f : %f]\n", g_rotGrid.x, g_rotGrid.y, g_rotGrid.z);
	DebugProc_Print((char*)"X����] : �� / ��\n");
	DebugProc_Print((char*)"Y����] : �p / �d\n");
	DebugProc_Print((char*)"Z����] : �� / ��\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"�ʒu�E�������Z�b�g : ENTER\n");
}

//=============================================================================
// �`�揈��
//=============================================================================
void Grid_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldGrid);

	//�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_sclGrid.x, g_sclGrid.y, g_sclGrid.z);
	D3DXMatrixMultiply(&g_mtxWorldGrid, &g_mtxWorldGrid, &mtxScl);

	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotGrid.y, g_rotGrid.x, g_rotGrid.z);
	D3DXMatrixMultiply(&g_mtxWorldGrid, &g_mtxWorldGrid, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posGrid.x, g_posGrid.y, g_posGrid.z);
	D3DXMatrixMultiply(&g_mtxWorldGrid, &g_mtxWorldGrid, &mtxTranslate);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldGrid);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffGrid, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, NUM_POLYGON);


}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGrid(LPDIRECT3DDEVICE9 pDevice)
{

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffGrid, NULL)))
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffGrid->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		for (int i = 0; i < LINE_MAX+1; i+=2) {
			pVtx[i].pos = D3DXVECTOR3(-540, 0, -500+i);
			pVtx[i+1].pos = D3DXVECTOR3(540, 0, -500+i);
		}

		//���_�J���[�̐ݒ�
		for (int i = 0; i < LINE_MAX+1; i++) {
			pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}


		//���_�f�[�^���A�����b�N����
		g_pVtxBuffGrid->Unlock();
	}
	return S_OK;

}

