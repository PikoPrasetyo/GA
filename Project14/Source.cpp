#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <bits/stdc++.h> 

using namespace std;

// Jumlah populasi setiap generasi
#define POPULATION_SIZE 1000

// valid genes
const string GENES = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .?/!";

// target
string TARGET;

// fungsi untuk menghasilkan peluang random 
int angka_random(int a, int b) {
	int range = (b - a) + 1;
	int random_angka = a + (rand() % range);
	return random_angka;
}

float angka_random_float(int a, int b) {
	int range = (b - a) + 1;
	int random_angka = a + (rand() % range);
	return random_angka;
}

// mutasi genetika
char mutasi_genetika() {
	int panjang = GENES.size();
	int r = angka_random(0, panjang - 1);
	return GENES[r];
}

// membentuk kromosom untuk individu
string buat_genome() {
	int panjang = TARGET.size();
	string genome = "";
	for (int i = 0; i < panjang; i++) {
		genome += mutasi_genetika();
	}
	return genome;
}

// Class merepresentasi suatu individu
class individu{
public:
	string kromosom;
	int fitness;
	individu(string kromosom);
	individu kawin(individu ortu2);
	int hitung_fitness();
};

individu::individu(string kromosom) {
	this->kromosom = kromosom;
	fitness = hitung_fitness();
}

// Individu kawin dan menghasilkan keturunan
individu individu::kawin(individu par2) {
	//kromosom dari keturunan
	string kromosom_keturunan = "";

	int panjang = kromosom.size();
	for (int i = 0; i < panjang; i++) {
		//membuat probabilitas alam
		float alam_prob = angka_random_float(0, 100) / 100;

		//jika probabilitas kurang dari 45%, turunan memperoleh gene
		//dari ortu1
		if (alam_prob < 0.45) {
			kromosom_keturunan += kromosom[i];
		}

		//jika probabilitas di antara 45% dan 90%, turunan memperoleh gene
		//dari ortu2
		else if (alam_prob < 0.9) {
			kromosom_keturunan += par2.kromosom[i];
		}

		//sisanya turunan memperoleh gene termutasi
		//hal ini menjaga diversitas
		else
			kromosom_keturunan += mutasi_genetika();
	}
	//individu baru terbentuk
	return individu(kromosom_keturunan);
};

//menghitung nilai fitness, nilai ini diukur dari banyaknya karakter
//pada string yang tidak sama dengan string target.
int individu::hitung_fitness() {
	int panjang = TARGET.size();
	int fitness = 0;
	for (int i = 0; i < panjang; i++) {
		if (kromosom[i] != TARGET[i]) {
			fitness++;
		}
	}
	return fitness;
};

//overloading < operator
bool operator<(const individu &ind1, const individu &ind2) {
	return ind1.fitness < ind2.fitness;
}

//Driver Code
int main() {

	//memasukan data target yang dituju
	cout << "memasukkan data berupa string : ";
	getline (cin, TARGET);

	srand((unsigned)time(NULL));

	//generasi saat ini
	int generation = 0;

	vector<individu> population;
	bool found = false;

	//menciptakan populasi awal
	for (int i = 0; i < POPULATION_SIZE; i++) {
		string genome = buat_genome();
		population.push_back(individu(genome));
	}

/*	sort(population.begin(), population.end());
	for (int i = 0; i < POPULATION_SIZE; i++) {
		cout << population[i].kromosom << "\t";
		cout << population[i].fitness << endl;
	}

	cout << "kawin awal" << "\t";
	for (int i = 0; i <= 5; i++) {
		cout << "\n";
	}

	vector<individu> population_generasi_selanjutnya;

	for (int i = 0; i < POPULATION_SIZE; i++) {
		int panjang = population.size();
		int random_05_best = angka_random(0, POPULATION_SIZE / 2);
		individu parent1 = population[random_05_best];
		cout << "ini ortu 1 : ";
		cout << population[random_05_best].kromosom << "\t";
		cout << population[random_05_best].fitness << endl;
		random_05_best = angka_random(0, POPULATION_SIZE / 2);
		individu parent2 = population[random_05_best];
		cout << "ini ortu 2 : ";
		cout << population[random_05_best].kromosom << "\t";
		cout << population[random_05_best].fitness << endl;
		individu anak = parent1.kawin(parent2);
		population_generasi_selanjutnya.push_back(anak);
		cout << "ini anak   : ";
		cout << population_generasi_selanjutnya[i].kromosom << "\t";
		cout << population_generasi_selanjutnya[i].fitness << endl;
		cout << "\n";
	}*/

	while (!found) {
		//urutkan populasi sesuai dengan fitness score
		sort(population.begin(), population.end());

		//jikalau sudah ada individu dengan fitness 0 
		//pencarian dihentikan
		if (population[0].fitness <= 0) {
			found = true;
			break;
		}

		//jika tidak ada individu dengan fitness 0
		// buat generasi baru dari seleksi alam
		vector<individu> population_generasi_selanjutnya;

		//melakukan elitism dimana 10% populasi pertama masuk
		//populasi baru
		int kaum_lolos_elite = (10 * POPULATION_SIZE) / 100;
		for (int i = 0; i < kaum_lolos_elite; i++) {
			population_generasi_selanjutnya.push_back(population[i]);
		}

		//50% individu terbaik diperbolehkan kawin
		int kaum_hasil_kawin = (90 * POPULATION_SIZE) / 100;
		for (int i = 0; i < kaum_hasil_kawin; i++)
		{
			int len = population.size();
			int random_05_best = angka_random(0, POPULATION_SIZE / 2);
			individu parent1 = population[random_05_best];
			random_05_best = angka_random(0, POPULATION_SIZE / 2);
			individu parent2 = population[random_05_best];
			individu offspring = parent1.kawin(parent2);
			population_generasi_selanjutnya.push_back(offspring);
		}
		population = population_generasi_selanjutnya;
		cout << "Generation: " << generation << "\t";
		cout << "String: " << population[0].kromosom << "\t";
		cout << "Fitness: " << population[0].fitness << "\n";

		generation++;
	}
	cout << "Generation: " << generation << "\t";
	cout << "String: " << population[0].kromosom << "\t";
	cout << "Fitness: " << population[0].fitness << "\n"; 
	_getch();
}