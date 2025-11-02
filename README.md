# TSP Heuristique : Kruskal → Euler → Hamiltonien

Ce projet implémente une heuristique classique pour résoudre le Problème du Voyageur de Commerce (TSP), basée sur :
- Construction d'un **arbre couvrant minimum (MST)** avec l'algorithme de **Kruskal**
- **Doublage des arêtes** pour rendre le graphe eulérien
- Parcours eulérien via l'algorithme de **Hierholzer**
- Transformation en **cycle hamiltonien** pour obtenir une solution du TSP

---

## 🚀 Fonctionnalités principales

- Génération de matrices de distances aléatoires vérifiant l’inégalité triangulaire
- Implémentation du MST via **Kruskal**
- Parcours eulérien et conversion en cycle hamiltonien
- Analyse des complexités pour chaque étape
- Visualisation des graphes avec Graphviz (si installés)

---

## 🧠 Algorithmes utilisés

- **Kruskal** (construction du MST)
- **Hierholzer** (parcours eulérien)
- **Suppression des répétitions** (pour obtenir cycle hamiltonien)

---

## 📊 Analyse de complexité

| Étape | Complexité |
|-------|------------|
| MST (Kruskal) | O(E log V) |
| Doublage des arêtes | O(E) |
| Parcours eulérien | O(E) |
| Transformation en cycle | O(V) |
| **Total** | Dominé par O(E log V) |

---

## 🖼️ Exemple d'exécution

```bash
# Générer un graphe aléatoire et lancer l'algorithme
python main.py --nodes 10
