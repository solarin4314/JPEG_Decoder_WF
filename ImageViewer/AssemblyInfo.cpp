#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;

//
// ��������� �Ϲ� ������ ���� Ư�� ������ ���� ����˴ϴ�.
// ��������� ���õ� ������ �����Ϸ���
// �� Ư�� ���� �����Ͻʽÿ�.
//
[assembly:AssemblyTitleAttribute("ImageViewer")];
[assembly:AssemblyDescriptionAttribute("")];
[assembly:AssemblyConfigurationAttribute("")];
[assembly:AssemblyCompanyAttribute("")];
[assembly:AssemblyProductAttribute("ImageViewer")];
[assembly:AssemblyCopyrightAttribute("Copyright (c)  2015")];
[assembly:AssemblyTrademarkAttribute("")];
[assembly:AssemblyCultureAttribute("")];

//
// ��������� ���� ������ ���� �� ���� ������ �����˴ϴ�.
//
//      �� ����
//      �� ����
//      ���� ��ȣ
//      ���� ����
//
// ��� ���� �����ϰų� �Ʒ��� ���� '*'�� ����Ͽ� ���� ��ȣ �� ���� ������ �ڵ�����
// �����ǵ��� �� �� �ֽ��ϴ�.

[assembly:AssemblyVersionAttribute("1.0.*")];

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];

[assembly:SecurityPermission(SecurityAction::RequestMinimum, UnmanagedCode = true)];