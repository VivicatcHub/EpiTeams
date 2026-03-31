# ✅ MyTeams — Checklist de développement

> Projet Epitech — Application de communication collaborative (like Microsoft Teams)
> Binaires : `myteams_server` | `myteams_cli` — Langage : C/C++ — Réseau : TCP sockets

---

## 🏗️ Général / Infrastructure

- [x] **Makefile** avec les règles `all`, `re`, `clean`, `fclean` pour les deux binaires
- [x] **Bibliothèque de logging** présente dans `libs/myteams/` à la racine du dépôt
  - [ ] Utiliser **uniquement** les fonctions de la lib pour afficher les événements (sur stderr)
  - [ ] Ne jamais écrire manuellement sur stderr — utiliser stdout pour les erreurs perso
- [ ] **RFC** — Rédiger une documentation de protocole au style RFC
  - [ ] Décrire le format des messages échangés entre client et serveur
  - [ ] Décrire les codes/types de paquets, leur structure binaire ou textuelle
  - [ ] Décrire les séquences d'échange (login, création, listing, etc.)
- [ ] **Modèle de données** conforme aux propriétés techniques de la bibliothèque fournie
  - [ ] Utiliser `uuid` (`man 3 uuid`) pour identifier chaque ressource (user, team, channel, thread, message)
  - [ ] Se documenter sur `man 7 queue` pour les structures de listes chaînées système

---

## 🖥️ Serveur (`myteams_server`)

### Démarrage & usage

- [ ] Vérifier les arguments : `./myteams_server <port>`
  - [ ] Afficher le message d'usage si `--help` ou arguments invalides
  - [ ] Le port doit être un entier valide

### Réseau — Multiplexage I/O avec `poll`

> 💡 `poll` permet de surveiller plusieurs file descriptors simultanément (écoute + clients connectés) sans bloquer. Il faut surveiller à la fois les événements de **lecture** (`POLLIN`) et d'**écriture** (`POLLOUT`) sur chaque socket.

- [ ] Créer une socket TCP en écoute sur le port donné
- [ ] Utiliser **`poll`** (et non `select` ou `epoll`) pour gérer plusieurs clients simultanément
  - [ ] Surveiller la socket serveur pour les nouvelles connexions
  - [ ] Surveiller chaque socket client en lecture (`POLLIN`)
  - [ ] Surveiller chaque socket client en écriture (`POLLOUT`) quand il y a des données à envoyer
  - [ ] Ne jamais utiliser `fork` ni threads — tout doit être dans une boucle événementielle unique
- [ ] Gérer la déconnexion propre d'un client (fermeture du fd, nettoyage de la liste)

### Persistance (Sauvegarde & Restauration)

> 💡 À la réception de `SIGINT` (Ctrl+C), le serveur doit sauvegarder son état dans des fichiers dans le dossier courant. Au redémarrage, il doit détecter ces fichiers et les charger.

- [ ] Intercepter le signal `SIGINT` pour déclencher la sauvegarde avant de quitter
- [ ] Sauvegarder dans le dossier courant :
  - [ ] Les utilisateurs (uuid, nom, statut connecté/déconnecté)
  - [ ] Les équipes (uuid, nom, description, liste des membres)
  - [ ] Les channels (uuid, nom, description, team parente)
  - [ ] Les threads (uuid, titre, message, channel parent, auteur, timestamp)
  - [ ] Les commentaires/replies (uuid, body, thread parent, auteur, timestamp)
  - [ ] Les messages privés (uuid, expéditeur, destinataire, body, timestamp)
- [ ] Au démarrage, vérifier si les fichiers de sauvegarde existent et les charger

### Sécurité & Contrôle d'accès

- [ ] Un client **non connecté** (pas encore `/login`) ne peut pas accéder aux ressources
  - [ ] Ne pas lui envoyer la liste des utilisateurs connectés
  - [ ] Rejeter toute commande autre que le login
- [ ] Un utilisateur **non abonné** à une équipe ne peut pas :
  - [ ] Créer un thread dans un channel de cette équipe
  - [ ] Recevoir les événements de cette équipe (nouveaux threads, etc.)
- [ ] Vérifier l'appartenance à une équipe avant chaque action sensible

### Gestion des événements (logging library)

- [ ] Appeler la fonction de log appropriée pour chaque événement :
  - [ ] Connexion / déconnexion d'un utilisateur
  - [ ] Création d'un utilisateur
  - [ ] Création d'une équipe
  - [ ] Abonnement / désabonnement à une équipe
  - [ ] Création d'un channel
  - [ ] Création d'un thread
  - [ ] Création d'un commentaire
  - [ ] Envoi / réception d'un message privé

### Features métier

- [ ] **Gestion des utilisateurs**
  - [ ] Créer un utilisateur lors du premier `/login` avec un nom donné (ou le reconnaître s'il existe déjà)
  - [ ] Marquer l'utilisateur comme connecté / déconnecté
- [ ] **Gestion des équipes**
  - [ ] Créer une équipe (nom + description, MAX_NAME_LENGTH=32, MAX_DESCRIPTION_LENGTH=255)
  - [ ] Permettre à un utilisateur de rejoindre une équipe
  - [ ] Permettre à un utilisateur de quitter une équipe
  - [ ] Lister toutes les équipes existantes
  - [ ] Lister les membres d'une équipe
- [ ] **Gestion des channels**
  - [ ] Créer un channel dans une équipe (nom + description)
  - [ ] Lister les channels d'une équipe
- [ ] **Gestion des threads**
  - [ ] Créer un thread dans un channel (titre + message, MAX_BODY_LENGTH=512)
  - [ ] Lister les threads d'un channel
- [ ] **Gestion des commentaires**
  - [ ] Créer un commentaire/reply dans un thread (body)
  - [ ] Lister les commentaires d'un thread
- [ ] **Messages privés**
  - [ ] Envoyer un message privé d'un utilisateur à un autre
  - [ ] Lister les messages échangés entre deux utilisateurs

---

## 💻 Client CLI (`myteams_cli`)

### Démarrage & usage

- [ ] Vérifier les arguments : `./myteams_cli <ip> <port>`
  - [ ] Afficher le message d'usage si `--help` ou arguments invalides
- [ ] Se connecter au serveur via TCP

### Réseau

- [ ] Lire les commandes depuis **stdin** et les envoyer au serveur
- [ ] Recevoir et afficher les réponses du serveur (via la bibliothèque de display)
- [ ] Utiliser `poll` pour gérer simultanément stdin et la socket serveur

### Parsing des commandes

> 💡 Toutes les commandes ont leurs arguments entre guillemets doubles. Une erreur de parsing (guillemet manquant, mauvais nombre d'arguments) doit être signalée.

- [ ] Parser correctement les arguments entre guillemets doubles `"..."`
- [ ] Retourner une erreur si un guillemet est manquant ou mal formé
- [ ] Respecter les limites de longueur :
  - [ ] `MAX_NAME_LENGTH` = 32
  - [ ] `MAX_DESCRIPTION_LENGTH` = 255
  - [ ] `MAX_BODY_LENGTH` = 512

### Contexte (`/use`)

> 💡 Le contexte est un état interne du client qui retient quel team/channel/thread est actuellement "sélectionné". Il influe sur le comportement des commandes `/create`, `/list` et `/info`.

- [ ] Maintenir un état de contexte local : `team_uuid`, `channel_uuid`, `thread_uuid` (tous optionnels)
- [ ] `/use` sans arguments → réinitialise le contexte
- [ ] `/use "team_uuid"` → contexte = équipe
- [ ] `/use "team_uuid" "channel_uuid"` → contexte = channel
- [ ] `/use "team_uuid" "channel_uuid" "thread_uuid"` → contexte = thread

### Commandes à implémenter

#### Commandes générales

- [ ] `/help` — Afficher l'aide avec toutes les commandes disponibles
- [ ] `/login "user_name"` — S'identifier auprès du serveur (crée l'utilisateur si nouveau)
- [ ] `/logout` — Se déconnecter proprement du serveur
- [ ] `/users` — Lister tous les utilisateurs existants sur le domaine
- [ ] `/user "user_uuid"` — Afficher les détails d'un utilisateur spécifique
- [ ] `/send "user_uuid" "message_body"` — Envoyer un message privé à un utilisateur
- [ ] `/messages "user_uuid"` — Lister tous les messages échangés avec un utilisateur

#### Commandes d'abonnement

- [ ] `/subscribe "team_uuid"` — S'abonner à une équipe (et recevoir ses événements)
- [ ] `/subscribed` — Lister toutes les équipes auxquelles on est abonné
- [ ] `/subscribed "team_uuid"` — Lister tous les utilisateurs abonnés à une équipe donnée
- [ ] `/unsubscribe "team_uuid"` — Se désabonner d'une équipe

#### Commande `/create` (contextuelle)

- [ ] Sans contexte → `/create "team_name" "team_description"` — Créer une équipe
- [ ] Contexte équipe → `/create "channel_name" "channel_description"` — Créer un channel
- [ ] Contexte channel → `/create "thread_title" "thread_message"` — Créer un thread
- [ ] Contexte thread → `/create "comment_body"` — Créer un commentaire/reply

#### Commande `/list` (contextuelle)

- [ ] Sans contexte → Lister toutes les équipes existantes
- [ ] Contexte équipe → Lister tous les channels de l'équipe
- [ ] Contexte channel → Lister tous les threads du channel
- [ ] Contexte thread → Lister tous les commentaires/replies du thread

#### Commande `/info` (contextuelle)

- [ ] Sans contexte → Afficher les détails de l'utilisateur actuellement connecté
- [ ] Contexte équipe → Afficher les détails de l'équipe sélectionnée
- [ ] Contexte channel → Afficher les détails du channel sélectionné
- [ ] Contexte thread → Afficher les détails du thread sélectionné

### Affichage

- [ ] Utiliser **uniquement** les fonctions de la bibliothèque de display fournie pour afficher les données
- [ ] Ne pas réinventer l'affichage manuellement

---

## 🧪 Debug & Qualité

- [ ] Tester avec **`strace`** pour vérifier les appels système (poll, read, write, etc.)
- [ ] Vérifier l'absence de fuites mémoire (valgrind)
- [ ] Tester la sauvegarde/restauration : lancer le serveur, créer des données, Ctrl+C, relancer → tout doit être restauré
- [ ] Tester la déconnexion brutale d'un client (fermeture du terminal)
- [ ] Tester plusieurs clients simultanés sur le même serveur
