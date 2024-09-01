#include "StdAfx.h"
#include "ChangePropertiesToByLayer.hpp"

void ChangePropertiesToByLayer::init()
{
    // 注册命令 CPTBL（change properties to ByLayer)
    acedRegCmds->addCommand(L"IYATTyxTools", L"CPTBL", L"CPTBL", ACRX_CMD_MODAL, ChangePropertiesToByLayer::change);
}

void ChangePropertiesToByLayer::unload()
{
    acedRegCmds->removeGroup(L"IYATTyxTools");
}

void ChangePropertiesToByLayer::change()
{
    acutPrintf(L"功能：修改选中对象的颜色、线型、线宽为 ByLayer，修改线型比例为 1。");
    ads_name ss = { 0, 0 }; // 选择集
    int ret = acedSSGet(nullptr, nullptr, nullptr, nullptr, ss); // 获取用户选择的对象集
    if (ret != RTNORM)
    {
        acutPrintf(L"\n没有选中任何对象。");
        return;
    }

    Adesk::Int32 length = 0; // 选择集长度
    acedSSLength(ss, &length); // 获取选择集的长度

    for (Adesk::Int32 i = 0; i < length; ++i) // 遍历选择集
    {
        ads_name name = { 0, 0 }; // 对象名
        acedSSName(ss, i, name); // 获取选择集中的第 i 个对象名

        AcDbObjectId objId; // 对象ID
        acdbGetObjectId(objId, name); // 获取对象 ID

        AcDbObject* pObj = nullptr; // 对象指针
        acdbOpenAcDbObject(pObj, objId, AcDb::kForWrite); // 以写权限打开对象

        AcDbEntity* pEntity = AcDbEntity::cast(pObj); // 强制转换为实体对象
        if (pEntity != nullptr)
        {
            pEntity->setColorIndex(256);  // 颜色设置为 ByLayer
            pEntity->setLinetype(L"ByLayer"); // 线型设置为 ByLayer
            pEntity->setLinetypeScale(1.0);  // 可选，默认线型比例
            pEntity->setLineWeight(AcDb::kLnWtByLayer); // 线宽设置为 ByLayer

            pEntity->downgradeOpen(); // 将对象改为只读模式
        }
        pObj->close(); // 关闭对象
    }

    acedSSFree(ss); // 释放选择集
    acutPrintf(L"选中对象已修改属性为 ByLayer。");
}