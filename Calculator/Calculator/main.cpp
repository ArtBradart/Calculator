#include <iostream>
#include <string>
#include "Expression.h"

using namespace std;

void UpdateSource(const string& source)
{
	Expression::Inst()->SetSource(source);

	if (Expression::Inst()->IsValid()) {
		double result = Expression::Inst()->GetResult();
		//NumberFormat nf = NumberFormat.getInstance();
		//nf.setMaximumFractionDigits(8);
		//mainWindow.SetResult(nf.format(result));
		// TODO: Show risult.
	}
	else
	{
		// TODO: Error.
		//mainWindow.SetError();
	}
}

int main(int argc, char** argv)
{
	while (true)
	{
		cout << "Input: ";
		string str;
		cin >> str;
		cout << "Oops...";
	}
	return 0;
}