#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

struct Arete {
    int sommet1, sommet2;
    int poids;

    bool operator<(const Arete& other) const {
        return poids < other.poids;
    }
};

struct UnionFind {
    vector<int> parent;

    UnionFind(int n) {
        parent.resize(n);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx != ry)
            parent[rx] = ry;
    }
};

vector<vector<int> > Simul(int n, int maxDistance = 100) {
    vector<vector<int> > distances(n, vector<int>(n, 0));
    srand(time(0));

    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j) {
            distances[i][j] = rand() % maxDistance + 1;
            distances[j][i] = distances[i][j];
        }

    for (int u = 0; u < n; ++u)
        for (int v = 0; v < n; ++v)
            for (int w = 0; w < n; ++w)
                if (u != v && v != w && u != w)
                    distances[u][w] = min(distances[u][w], distances[u][v] + distances[v][w]);

    return distances;
}

vector<Arete> construireAretes(const vector<vector<int> >& distances) {
    int n = distances.size();
    vector<Arete> aretes;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            aretes.push_back({i, j, distances[i][j]});
    return aretes;
}

vector<Arete> kruskal(vector<Arete>& aretes) {
    vector<Arete> arbre;
    sort(aretes.begin(), aretes.end());
    UnionFind uf(aretes.size());

    for (const auto& a : aretes) {
        if (uf.find(a.sommet1) != uf.find(a.sommet2)) {
            arbre.push_back(a);
            uf.unite(a.sommet1, a.sommet2);
        }
    }

    return arbre;
}

void construireArbre(vector<vector<int> >& adj, const vector<Arete>& arbre) {
    for (const auto& a : arbre) {
        adj[a.sommet1].push_back(a.sommet2);
        adj[a.sommet2].push_back(a.sommet1);
    }
}

void dfs(int u, const vector<vector<int> >& adj, vector<bool>& visite, vector<int>& parcours) {
    visite[u] = true;
    parcours.push_back(u);
    for (int v : adj[u])
        if (!visite[v])
            dfs(v, adj, visite, parcours);
}

vector<int> toHamiltonien(const vector<int>& parcours) {
    vector<bool> visite(*max_element(parcours.begin(), parcours.end()) + 1, false);
    vector<int> cycle;

    for (int v : parcours) {
        if (!visite[v]) {
            cycle.push_back(v);
            visite[v] = true;
        }
    }

    if (!cycle.empty())
        cycle.push_back(cycle.front()); // boucle pour fermer le cycle

    return cycle;
}

int calculerLongueurCycle(const vector<int>& cycle, const vector<vector<int> >& distances) {
    int longueur = 0;
    for (size_t i = 0; i < cycle.size() - 1; ++i)
        longueur += distances[cycle[i]][cycle[i + 1]];
    return longueur;
}

void exporterDot(const vector<Arete>& aretes, const string& nomFichier, const string& nomGraphe) {
    ofstream fichier(nomFichier);
    if (!fichier) {
        cerr << "Erreur lors de la création du fichier DOT." << endl;
        return;
    }

    fichier << "graph " << nomGraphe << " {\n";
    for (const auto& a : aretes) {
        fichier << "    \"" << a.sommet1 << "\" -- \"" << a.sommet2 << "\" [label=" << a.poids << "];\n";
    }
    fichier << "}\n";
    fichier.close();
    cout << "Fichier DOT généré : " << nomFichier << endl;
}

void exporterDistances(const vector<vector<int>>& distances, const string& nomFichier) {
    ofstream fichier(nomFichier);
    if (!fichier) {
        cerr << "Erreur lors de la création du fichier des distances." << endl;
        return;
    }

    int n = distances.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fichier << distances[i][j];
            if (j != n - 1) fichier << ",";
        }
        fichier << "\n";
    }

    fichier.close();
    cout << "Fichier distances généré : " << nomFichier << endl;
}

void exporterResultats(const vector<int>& cycle, int longueur, const string& nomFichier) {
    ofstream fichier(nomFichier);
    if (!fichier) {
        cerr << "Erreur lors de la création du fichier des résultats." << endl;
        return;
    }

    fichier << "Cycle Hamiltonien :\n";
    for (size_t i = 0; i < cycle.size(); ++i) {
        fichier << cycle[i];
        if (i != cycle.size() - 1) fichier << " -> ";
    }
    fichier << "\n\nLongueur totale : " << longueur << "\n";
    fichier.close();
    cout << "Fichier résultats généré : " << nomFichier << endl;
}


int main() {
    int taillesArray[] = {10, 20, 50};
    vector<int> tailles(taillesArray, taillesArray + sizeof(taillesArray) / sizeof(int));

    for (int k = 0; k < tailles.size(); ++k) {
        int n = tailles[k];
        cout << "\n========== Test pour n = " << n << " ==========\n";

        clock_t debut = clock();

        // 1. Génération des distances
        vector<vector<int>> distances = Simul(n);
        exporterDistances(distances, "distances_" + to_string(n) + ".csv");


        // 2. Construction des arêtes
        vector<Arete> aretes = construireAretes(distances);

        // 3. Exporter le graphe initial
        exporterDot(aretes, "graphe_initial_" + to_string(n) + ".dot", "GrapheInitial" + to_string(n));

        // 4. Kruskal
        vector<Arete> arbre = kruskal(aretes);

        // 5. Exporter l'arbre de Kruskal
        exporterDot(arbre, "arbre_kruskal_" + to_string(n) + ".dot", "ArbreKruskal" + to_string(n));

        // 6. Construction de l'arbre en liste d'adjacence
        vector<vector<int>> adj(n);
        construireArbre(adj, arbre);

        // 7. DFS pour parcours eulérien
        vector<bool> visite(n, false);
        vector<int> parcours;
        dfs(0, adj, visite, parcours);

        // 8. Transformation en cycle hamiltonien
        vector<int> cycle = toHamiltonien(parcours);
        cycle.push_back(cycle[0]); // pour boucler

        // 9. Calcul de la longueur
        int longueur = calculerLongueurCycle(cycle, distances);
        cout << "Longueur du cycle hamiltonien : " << longueur << endl;
        exporterResultats(cycle, longueur, "resultats_" + to_string(n) + ".txt");


        // 10. Affichage du cycle
        cout << "Cycle : ";
        for (int i = 0; i < cycle.size(); ++i) {
            cout << cycle[i];
            if (i != cycle.size() - 1) cout << " -> ";
        }
        cout << endl;

        // 11. Temps d'exécution
        clock_t fin = clock();
        double temps = double(fin - debut) / CLOCKS_PER_SEC;
        cout << "Temps d'exécution : " << fixed << setprecision(4) << temps << " secondes" << endl;
        cout << "==========================================\n";
    }

	system("pause");
    return 0;
}

