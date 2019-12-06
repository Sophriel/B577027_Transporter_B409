//====================================================================================
//		## SkinModel ## (�ϵ���� ��Ű�� �� Ŭ����)
//====================================================================================
#pragma once

#include "Animation.h"

class SkinModel
{
public:
	SkinModel();
	virtual ~SkinModel();

	void					Release();
	void					Render(ID3D11DeviceContext* dc);
	
	//Animation Function
	void					PlayAni(int idx);
	void					StopAni()			{	_aniList[_playAniIdx].Stop();	}
	void					PauseAni()			{	_aniList[_playAniIdx].Pause();	}


	//���׸��� ���� �޽����� ������Ʈ
	void					UpdateMeshByMaterial();


	//Get Function
	vector<Mesh*>&	GetMeshList()		{	return _meshList;				}
	//vector<NodeInfo*>&		GetNodeList()		{	return _nodeList;				}

	//vector<Material>&		GetMaterialList()	{	return _materialList;			}
	vector<Animation>&		GetAnimationList()	{	return _aniList;				}

protected:
	void UpdateNodeTM();

protected:
	//Shader*					_texShader		= NULL;
	//Shader*					_skinShader		= NULL;
	int						_playAniIdx		= -1;

	//vector<Material>		_materialList;
	vector<Animation>		_aniList;
	//vector<NodeInfo*>		_nodeList;
	vector<Mesh*>	_meshList;

	//���׸��� ���� �������� ���� ����, ���ǻ� �޽� ������ �и� �� ��.
	vector<vector<Mesh*>> _meshByMaterial;
};
