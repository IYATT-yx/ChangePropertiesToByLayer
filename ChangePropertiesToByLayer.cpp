#include "StdAfx.h"
#include "ChangePropertiesToByLayer.hpp"

void ChangePropertiesToByLayer::init()
{
    // ע������ CPTBL��change properties to ByLayer)
    acedRegCmds->addCommand(L"IYATTyxTools", L"CPTBL", L"CPTBL", ACRX_CMD_MODAL, ChangePropertiesToByLayer::change);
}

void ChangePropertiesToByLayer::unload()
{
    acedRegCmds->removeGroup(L"IYATTyxTools");
}

void ChangePropertiesToByLayer::change()
{
    acutPrintf(L"���ܣ��޸�ѡ�ж������ɫ�����͡��߿�Ϊ ByLayer���޸����ͱ���Ϊ 1��");
    ads_name ss = { 0, 0 }; // ѡ��
    int ret = acedSSGet(nullptr, nullptr, nullptr, nullptr, ss); // ��ȡ�û�ѡ��Ķ���
    if (ret != RTNORM)
    {
        acutPrintf(L"\nû��ѡ���κζ���");
        return;
    }

    Adesk::Int32 length = 0; // ѡ�񼯳���
    acedSSLength(ss, &length); // ��ȡѡ�񼯵ĳ���

    for (Adesk::Int32 i = 0; i < length; ++i) // ����ѡ��
    {
        ads_name name = { 0, 0 }; // ������
        acedSSName(ss, i, name); // ��ȡѡ���еĵ� i ��������

        AcDbObjectId objId; // ����ID
        acdbGetObjectId(objId, name); // ��ȡ���� ID

        AcDbObject* pObj = nullptr; // ����ָ��
        acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite); // ��дȨ�޴򿪶���

        AcDbEntity* pEntity = AcDbEntity::cast(pObj); // ǿ��ת��Ϊʵ�����
        if (pEntity != nullptr)
        {
            pEntity->setColorIndex(256);  // ��ɫ����Ϊ ByLayer
            pEntity->setLinetype(L"ByLayer"); // ��������Ϊ ByLayer
            pEntity->setLinetypeScale(1.0);  // ��ѡ��Ĭ�����ͱ���
            pEntity->setLineWeight(AcDb::kLnWtByLayer); // �߿�����Ϊ ByLayer

            pEntity->downgradeOpen(); // �������Ϊֻ��ģʽ
        }
        pObj->close(); // �رն���
    }

    acedSSFree(ss); // �ͷ�ѡ��
    acutPrintf(L"ѡ�ж������޸�����Ϊ ByLayer��");
}