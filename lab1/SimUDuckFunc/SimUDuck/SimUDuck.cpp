#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <functional>

using namespace std;
using FlyStrategy = function<void()>;
using QuackStrategy = function<void()>;
using DanceStrategy = function<void(int speed)>;

function<void()> FlyWithWings()
{
	int amountSortie = 0;
	cout << "I'm flying with wings!!!" << endl;
	return [amountSortie]() mutable {
		++amountSortie;
		cout << "Amount sortie: " << amountSortie << endl;
	};
};

void FlyNoWay (){};

void QuackBehavior ()
{
	cout << "Quack Quack!!!" << endl;
};

void SqueakBehavior()
{
	cout << "Squeek!!!" << endl;
};

void MuteQuackBehavior() {};

void DanceWaltz(int speed)
{
	cout << "I am dancing waltz, with speed = " << speed << endl;
};

void DanceMinuet(int speed)
{
	cout << "I am dancing waltz minuet, with speed = " << speed << endl;
};

void NoDance(int speed) {};

class Duck
{
public:
	Duck(FlyStrategy&& flyBehavior,
		QuackStrategy&& quackBehavior,
		DanceStrategy&& danceBehavior)
		: m_quackBehavior(move(quackBehavior)),
		m_danceBehavior(move(danceBehavior))
	{
		assert(m_quackBehavior);
		assert(m_danceBehavior);
		SetFlyBehavior(move(flyBehavior));
	}
	void Quack() const
	{
		m_quackBehavior();
	}
	void Swim()
	{
		cout << "I'm swimming" << endl;
	}
	void Fly()
	{
		m_flyBehavior();
	}
	void Dance(int speed)
	{
		m_danceBehavior(speed);
	}
	void SetFlyBehavior(FlyStrategy&& flyBehavior)
	{
		assert(flyBehavior);
		m_flyBehavior = move(flyBehavior);
	}
	virtual void Display() const = 0;
	virtual ~Duck() = default;

private:
	FlyStrategy m_flyBehavior;
	QuackStrategy m_quackBehavior;
	DanceStrategy m_danceBehavior;
};

class MallardDuck : public Duck
{
public:
	MallardDuck()
		: Duck(FlyWithWings(), QuackBehavior, DanceWaltz)
	{
	}

	void Display() const override
	{
		cout << "I'm mallard duck" << endl;
	}
};

class RedheadDuck : public Duck
{
public:
	RedheadDuck()
		: Duck(FlyWithWings(), QuackBehavior, DanceMinuet)
	{
	}
	void Display() const override
	{
		cout << "I'm redhead duck" << endl;
	}
};

class DecoyDuck : public Duck
{
public:
	DecoyDuck()
		: Duck(FlyNoWay, MuteQuackBehavior, NoDance)
	{
	}
	void Display() const override
	{
		cout << "I'm decoy duck" << endl;
	}
};
class RubberDuck : public Duck
{
public:
	RubberDuck()
		: Duck(FlyNoWay, SqueakBehavior, NoDance)
	{
	}
	void Display() const override
	{
		cout << "I'm rubber duck" << endl;
	}
};

class ModelDuck : public Duck
{
public:
	ModelDuck()
		: Duck(FlyNoWay, QuackBehavior, NoDance)
	{
	}
	void Display() const override
	{
		cout << "I'm model duck" << endl;
	}
};

void DrawDuck(Duck const& duck)
{
	duck.Display();
}

void PlayWithDuck(Duck& duck)
{
	DrawDuck(duck);
	duck.Quack();
	duck.Fly();
	duck.Dance(5);
	cout << endl;
}

void main()
{
	MallardDuck mallardDuck;
	PlayWithDuck(mallardDuck);

	RedheadDuck redheadDuck;
	PlayWithDuck(redheadDuck);

	RubberDuck rubberDuck;
	PlayWithDuck(rubberDuck);

	DecoyDuck decoyDuck;
	PlayWithDuck(decoyDuck);

	ModelDuck modelDuck;
	PlayWithDuck(modelDuck);
	modelDuck.SetFlyBehavior(FlyWithWings);
	PlayWithDuck(modelDuck);
}