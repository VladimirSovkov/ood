#include "Beverages.h"
#include "Condiments.h"
#include <iostream>
#include <string>
#include <functional>

using namespace std;


/*
�������������� ������, ��������� �������� �������
*/
struct MakeLemon
{
	MakeLemon(unsigned quantity)
		:m_quantity(quantity)
	{}

	auto operator()(IBeveragePtr&& beverage)const
	{
		return make_unique<CLemon>(move(beverage), m_quantity);
	}
private:
	unsigned m_quantity;
};

/*
�������, ������������ �������, ��������� �������� �������
*/
function<IBeveragePtr(IBeveragePtr&&)> MakeCinnamon()
{
	return [](IBeveragePtr&& b) {
		return make_unique<CCinnamon>(move(b));
	};
}

/*
���������� �������, ������������ ������� ������������ ��������
��������� �������:
	Condiment - ����� �������, ����������� �������� � �������� ������� ���������
				��������� IBeveragePtr&& �������������� �������
	Args - ������ ����� ������ ���������� ������������ (��������, ������)
*/
template <typename Condiment, typename... Args>
auto MakeCondiment(const Args&...args)
{
	// ���������� �������, ������������ �������, ���������� �� � �������� ���������
	// �������������� ��������� ����������, ����������� ������-��������, ����������
	// ������������ ���������� ����� make_unique
	return [=](auto&& b) {
		// ������� make_unique �������� b ������ �� ������� ���������� ������� �������
		return make_unique<Condiment>(forward<decltype(b)>(b), args...);
	};
}

/*
������������� ������ ��������� <<, ������� ������������� ��� �������������� �����
��� ������������� ����������
��������� ������� ������� ������������� ������� ����������� ��������� �������:
auto beverage = make_unique<CConcreteBeverage>(a, b, c)
					<< MakeCondimentA(d, e, f)
					<< MakeCondimentB(g, h);
�������������� ������� MakeCondiment* ���������� ���������, ����������� ��� ��������
����������, � ���������� ��������� �������, ����������� ������������� �������, �������
��� ����� ������ ������� ������ ������ Condiment, ������� ��� ����������� ���������.
�������������:
	auto beverage =
		make_unique<CConcreteBeverage>(a, b, c)
		<< MakeCondimentA(d, e, f)
		<< MakeCondimentB(g, h);
��� ���� ���:
	auto beverage =
		make_unique<CConcreteBeverage>
		<< MakeCondiment<CondimentA>(d, e, f)
		<< MakeCondiment<CondimentB>(g, h);
� ��������� ������ ��� ������������� ������ ������� ���������� MakeCondimentA � MakeCondimentB, �.�.
����������� ���������� ����������� ����������
������������ ������ ������������ �������� �� ���:
	auto baseBeverage = make_unique<CConcretedBeverage>(a, b, c);
	auto wrappedWithCondimentA = make_unique<CCondimentA>(move(baseBeverage), d, e, f);
	auto beverage = make_unique<CCondimentB>(move(wrappedWithCondimentA), g, h);
���� ���:
	auto beverage = make_unique<CCondimentB>(
						make_unique<CCondimentA>(
							make_unique<CConcreteBeverage>(a, b, c), // �������
							d, e, f	// ���. �������� CondimentA
						),
						g, h		// ���. ��������� CondimentB
					);
unique_ptr<CLemon> operator << (IBeveragePtr && lhs, const MakeLemon & factory)
{
	return factory(move(lhs));
}
unique_ptr<CCinnamon> operator << (IBeveragePtr && lhs, const MakeCinnamon & factory)
{
	return factory(move(lhs));
}
*/
template <typename Component, typename Decorator>
auto operator << (Component&& component, const Decorator& decorate)
{
	return decorate(forward<Component>(component));
}

void DialogWithUser()
{
	cout << "Type 1 for coffee or 2 for tea\n";
	int beverageChoice;
	cin >> beverageChoice;

	unique_ptr<IBeverage> beverage;

	if (beverageChoice == 1)
	{
		beverage = make_unique<CCoffee>();
	}
	else if (beverageChoice == 2)
	{
		beverage = make_unique<CTea>();
	}
	else
	{
		return;
	}

	int condimentChoice;
	for (;;)
	{
		cout << "1 - Lemon, 2 - Cinnamon, 0 - Checkout" << endl;
		cin >> condimentChoice;

		if (condimentChoice == 1)
		{
			//beverage = make_unique<CLemon>(move(beverage));
			beverage = move(beverage) << MakeCondiment<CLemon>(2);
		}
		else if (condimentChoice == 2)
		{
			//beverage = make_unique<CCinnamon>(move(beverage));
			beverage = move(beverage) << MakeCondiment<CCinnamon>();
		}
		else if (condimentChoice == 0)
		{
			break;
		}
		else
		{
			return;
		}
	}



	cout << beverage->GetDescription() << ", cost: " << beverage->GetCost() << endl;
}

void Demonstration()
{
	cout << "Demonstration" << endl;

	//������ ����� 
	{
		auto latte = make_unique<CLatte>();
		cout << latte->GetDescription() << ", cost: " << latte->GetCost() << endl;
		latte = make_unique<CLatte>(LattePortion::Double);
		cout << latte->GetDescription() << ", cost: " << latte->GetCost() << endl;
	}
	
	//������ �������� 
	{
		auto cappuccino = make_unique<CCappuccino>();
		cout << cappuccino->GetDescription() << ", cost: " << cappuccino->GetCost() << endl;
		cappuccino = make_unique<CCappuccino>(CappuccinoPortion::Double);
		cout << cappuccino->GetDescription() << ", cost: " << cappuccino->GetCost() << endl;
	}

	//��������� ���� ���
	{
		auto blackTea = make_unique<CTea>(TeaType::GreanTea);
		cout << blackTea->GetDescription() << ", cost: " << blackTea->GetCost() << endl;
	}

	//��������� ������ �������� ���������
	{
		auto milkshake = make_unique<CMilkshake>();
		cout << milkshake->GetDescription() << ", cost: " << milkshake->GetCost() << endl;
		milkshake = make_unique<CMilkshake>(MilkshakeType::Large);
		cout << milkshake->GetDescription() << ", cost: " << milkshake->GetCost() << endl;
	}

	//������
	{
		auto latte = 
			make_unique<CLatte>()
			<< MakeCondiment<CCream>();
		cout << latte->GetDescription() << ", cost: " << latte->GetCost() << endl;
	}

	//�������
	{
		auto cappuccino =
			make_unique<CCappuccino>()
			<< MakeCondiment<CChocolate>(4);
		cout << cappuccino->GetDescription() << ", cost: " << cappuccino->GetCost() << endl;
	}

	//�����
	{
		auto cappuccino =
			make_unique<CCappuccino>()
			<< MakeCondiment<CLiquor>(LiquorType::�hocolate);
		cout << cappuccino->GetDescription() << ", cost: " << cappuccino->GetCost() << endl;
	}
}

int main()
{
	//DialogWithUser();
	Demonstration();
	cout << endl;
	{
		// �������� ������� �����
		auto latte = make_unique<CLatte>();
		// ��������� ������
		auto cinnamon = make_unique<CCinnamon>(move(latte));
		// ��������� ���� ����� ������
		auto lemon = make_unique<CLemon>(move(cinnamon), 2);
		// ��������� ���� ������� ����
		auto iceCubes = make_unique<CIceCubes>(move(lemon), 2, IceCubeType::Dry);
		// ��������� 2 ������ ���������� ������
		auto beverage = make_unique<CChocolateCrumbs>(move(iceCubes), 2);

		// ���������� ���� ����������
		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}

	{
		auto beverage =
			make_unique<CChocolateCrumbs>(			// ������� ����: ���������� ������
				make_unique<CIceCubes>(				// | ��� ��� - ������ ����
					make_unique<CLemon>(			// | | ��� ���� �����
						make_unique<CCinnamon>(		// | | | ����� ���� - ������
							make_unique<CLatte>()),	// | | |   � ����� ������ - �����
						2),							// | | 2 ������ ������
					2, IceCubeType::Dry),			// | 2 ������ ������ ����
				2);									// 2 ������ ���������� ������

		// ���������� ���� ����������
		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}

	// ��������� ���������� ������ MakeCondiment � ��������� <<
	{
		// lemon - �������, ����������� "2 ������ ������" � ������ �������
		auto lemon2 = MakeCondiment<CLemon>(2);
		// iceCubes - �������, ����������� "3 ������� ����" � ������ �������
		auto iceCubes3 = MakeCondiment<CIceCubes>(3, IceCubeType::Water);

		auto tea = make_unique<CTea>();

		// ���������� ��� ����� �������� ������ � ����� ��������� ����
		auto lemonIceTea = iceCubes3(lemon2(move(tea)));
		/* ���������� ������ ��������� �� �� ��������, ��� � ��������� ���:
		auto lemonIceTea =
			make_unique<CIceCubes>(
				make_unique<CLemon>(
					move(tea),
					2),
				2, IceCubeType::Water);
		*/

		auto oneMoreLemonIceTea =
			make_unique<CTea>()	// ����� ���
			<< MakeCondiment<CLemon>(2)	// ��������� ���� ����� ������
			<< MakeCondiment<CIceCubes>(3, IceCubeType::Water); // � 3 ������ ����
		/*
		���������� ����������� ������ �� �� �����, ��� � ���������:
		auto oneMoreLemonIceTea =
			MakeCondiment<CIceCubes>(3, IceCubeType::Water)(
				MakeCondiment<CLemon>(2)(make_unique<CTea>())
				);
		*/
	}

	// ������ ����������� ������� � �������� ��������������� ������
	// ��������������� ����������� << � �������� MakeCondiment
	{
		auto beverage =
			make_unique<CLatte>()							// �������� ������� �����,
			<< MakeCondiment<CCinnamon>()					// ����������� �������,
			<< MakeCondiment<CLemon>(2)						// ��������� ���� ����� ������
			<< MakeCondiment<CIceCubes>(2, IceCubeType::Dry)// ������� ���� ������� ������ ����
			<< MakeCondiment<CChocolateCrumbs>(2);			// �������� ���������� �������

		// ���������� ���� ����������
		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}

	{
		auto beverage =
			make_unique<CMilkshake>()					// �������� �������� ��������
			<< MakeCondiment<CSyrup>(SyrupType::Maple)	// �������� �������� �������
			<< MakeCondiment<CCoconutFlakes>(8);		// �������� ��������� ��������

		// ���������� ���� ����������
		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}
}