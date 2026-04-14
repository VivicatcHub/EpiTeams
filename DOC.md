# MyTeams Documentation

## Table of Contents

- [Introduction](#introduction)
- [Commands](#commands)
  - [Help](#help)
  - [Log](#log)
    - [Login](#login)
    - [Logout](#logout)
  - [User Info](#user-info)
    - [Users](#users)
    - [User](#user)
  - [Message](#message)
    - [Send Message](#send-message)
    - [Messages](#messages)
  - [Subscription](#subscription)
    - [Subscribe](#subscribe)
    - [Subscribed](#subscribed)
    - [Unsubscribe](#unsubscribe)
  - [Context](#context)
    - [Use](#use)
    - [Create](#create)
    - [List](#list)
    - [Info](#info)

## Introduction

## Commands

### Help (`/help`)

> Show Help with all available commands

- Args: None
- Server:
  - Send: None
  - Return: None

### Log

#### Login (`/login`)

> Set the user_name used by client to identify itself to the server. If the user_name does not exist, it is created.

- Args: `["user_name"]`
- Server:
  - Send: `LOGIN <user_name>`
  - Return: `230`/`501`/`503`/`540`

Args:

| Argument    | Type   | Is Mandatory | Description                       |
| ----------- | ------ | ------------ | --------------------------------- |
| `user_name` | string | Yes          | The name of the user to log in as |

Return:

| Code | Description                               |
| ---- | ----------------------------------------- |
| 230  | Login successful, user is now logged in   |
| 501  | Login failed, user_name is incorect       |
| 503  | Login failed, user is already logged in   |
| 540  | Login failed, user_name is already in use |

#### Logout (`/logout`)

> Disconnect the client from the server

- Args: None
- Server:
  - Send: `LOGOUT`
  - Return: `231`/`530`

Return:

| Code | Description                               |
| ---- | ----------------------------------------- |
| 231  | Logout successful, user is now logged out |
| 530  | Logout failed, user is not logged in      |

### User Info

#### Users (`/users`)

> Get the list of all users that exist on the domain

- Args: None
- Server:
  - Send: `USERS`
  - Return: `201`->`<users-list>`->`202`/`530`

Return:

| Code | Description                                        |
| ---- | -------------------------------------------------- |
| 201  | Success, followed by the users list (can be empty) |
| 202  | End of users list                                  |
| 530  | Request failed, user is not logged in              |

#### User (`/user`)

> Get details about the requested user

- Args: `["user_uuid"]`
- Server:
  - Send: `USER <user_uuid>`
  - Return: `201`->`<user-info>`->`202`/`404`/`501`/`530`

Args:

| Argument    | Type   | Is Mandatory | Description                            |
| ----------- | ------ | ------------ | -------------------------------------- |
| `user_uuid` | string | Yes          | The UUID of the user to get info about |

Return:

| Code | Description                               |
| ---- | ----------------------------------------- |
| 201  | Success, followed by the user info        |
| 202  | End of user info                          |
| 404  | User not found with the given `user_uuid` |
| 501  | Request failed, user_uuid is incorrect    |
| 530  | Request failed, user is not logged in     |

### Message

#### Send Message (`/send`)

> Send a message to specific user identified by `user_uuid`

- Args: `["user_uuid"]` `["message_body"]`
- Server:
  - Send: `SEND <user_uuid> "<message_body>"`
  - Return: `200`/`404`/`501`/`530`

Args:

| Argument       | Type   | Is Mandatory | Description                                 |
| -------------- | ------ | ------------ | ------------------------------------------- |
| `user_uuid`    | string | Yes          | The UUID of the user to send the message to |
| `message_body` | string | Yes          | The content of the message                  |

Return:

| Code | Description                               |
| ---- | ----------------------------------------- |
| 200  | Message sent successfully                 |
| 404  | User not found with the given `user_uuid` |
| 501  | Request failed, message body is incorrect |
| 530  | Request failed, user is not logged in     |

#### Receive Message (no command)

> Receive a message from another user (sent by the `/send` command)

- Server:
  - Send: None
  - Return: `225`

| Code | Description                        |
| ---- | ---------------------------------- |
| 225  | Message received from another user |

#### Messages (`/messages`)

> List all messages exchanged with the specified user

- Args: `["user_uuid"]`
- Server:
  - Send: `MESSAGES <user_uuid>`
  - Return: `201`->`<messages-list>`->`202`/`404`/`501`/`530`

Args:

| Argument    | Type   | Is Mandatory | Description                                |
| ----------- | ------ | ------------ | ------------------------------------------ |
| `user_uuid` | string | Yes          | The UUID of the user to list messages with |

Return:

| Code | Description                               |
| ---- | ----------------------------------------- |
| 201  | Success, followed by the messages list    |
| 202  | End of messages                           |
| 404  | User not found with the given `user_uuid` |
| 501  | Request failed, user_uuid is incorrect    |
| 530  | Request failed, user is not logged in     |

### Subscription

#### Subscribe (`/subscribe`)

> Subscribe to the events of a team and its sub directories (enable reception of all events from a team)

- Args: `["team_uuid"]`
- Server:
  - Send: `SUBSCRIBE <team_uuid>`
  - Return: `200`/`404`/`530`

Args:

| Argument    | Type   | Is Mandatory | Description                          |
| ----------- | ------ | ------------ | ------------------------------------ |
| `team_uuid` | string | Yes          | The UUID of the team to subscribe to |

Return:

| Code | Description                               |
| ---- | ----------------------------------------- |
| 200  | Subscription successful                   |
| 404  | Team not found with the given `team_uuid` |
| 530  | Request failed, user is not logged in     |

#### Subscribed (`/subscribed`)

> List all subscribed teams or list all users subscribed to a team

- Args: `?["team_uuid"]`
- Server:
  - Send: `SUBSCRIBED`/`SUBSCRIBED <team_uuid>`
  - Return: `200`->`<subscribed-list>`/`200`->`<team-members>`/`404`/`530`

Args:

| Argument    | Type   | Is Mandatory | Description                                 |
| ----------- | ------ | ------------ | ------------------------------------------- |
| `team_uuid` | string | No           | The UUID of the team to list subscribers of |

Return:

| Code | Description                                       |
| ---- | ------------------------------------------------- |
| 200  | Success, followed by the list of subscribed teams |
| 200  | Success, followed by the list of team subscribers |
| 404  | Team not found with the given `team_uuid`         |
| 530  | Request failed, user is not logged in             |

#### Unsubscribe (`/unsubscribe`)

> Unsubscribe from a team (disable reception of all events from a team)

- Args: `["team_uuid"]`
- Server:
  - Send: `UNSUBSCRIBE <team_uuid>`
  - Return: `200`/`404`/`530`

Args:

| Argument    | Type   | Is Mandatory | Description                              |
| ----------- | ------ | ------------ | ---------------------------------------- |
| `team_uuid` | string | Yes          | The UUID of the team to unsubscribe from |

Return:

| Code | Description                               |
| ---- | ----------------------------------------- |
| 200  | Unsubscription successful                 |
| 404  | Team not found with the given `team_uuid` |
| 530  | Request failed, user is not logged in     |

### Context

#### Use (`/use`)

> Sets the command context to a team/channel/thread

- Args: `?["team_uuid"]` `?["channel_uuid"]` `?["thread_uuid"]`
- Server:
  - Send: None
  - Return: None

Args:

| Argument       | Type   | Is Mandatory | Description                                                                        |
| -------------- | ------ | ------------ | ---------------------------------------------------------------------------------- |
| `team_uuid`    | string | No           | The UUID of the team to set as context (if not provided, context is reset)         |
| `channel_uuid` | string | No           | The UUID of the channel to set as context (requires `team_uuid`)                   |
| `thread_uuid`  | string | No           | The UUID of the thread to set as context (requires `team_uuid` and `channel_uuid`) |

#### Create (`/create`)

##### Root

> Create a new team

- Args: `["team_name"]` `["team_description"]`
- Server:
  - Send: `CREATE_TEAM <team_name> "<team_description>"`
  - Return: `200`/`501`/`530`

Args:

| Argument           | Type   | Is Mandatory | Description                           |
| ------------------ | ------ | ------------ | ------------------------------------- |
| `team_name`        | string | Yes          | The name of the team to create        |
| `team_description` | string | Yes          | The description of the team to create |

Return:

| Code | Description                            |
| ---- | -------------------------------------- |
| 200  | Team created successfully              |
| 501  | Request failed, team_name is incorrect |
| 530  | Request failed, user is not logged in  |

##### Team Context

> Create a new channel

- Args: `["channel_name"]` `["channel_description"]`
- Server:
  - Send: `CREATE_CHANNEL <team_uuid> <channel_name> "<channel_description>"`
  - Return: `200`/`404`/`501`/`530`

Args:

| Argument              | Type   | Is Mandatory | Description                              |
| --------------------- | ------ | ------------ | ---------------------------------------- |
| `channel_name`        | string | Yes          | The name of the channel to create        |
| `channel_description` | string | Yes          | The description of the channel to create |

Return:

| Code | Description                               |
| ---- | ----------------------------------------- |
| 200  | Channel created successfully              |
| 404  | Team not found with the given `team_uuid` |
| 501  | Request failed, channel_name is incorrect |
| 530  | Request failed, user is not logged in     |

##### Channel Context

> Create a new thread

- Args: `["thread_title"]` `["thread_message"]`
- Server:
  - Send: `CREATE_THREAD <team_uuid> <channel_uuid> <thread_title> "<thread_message>"`
  - Return: `200`/`404`/`405`/`530`

Args:

| Argument         | Type   | Is Mandatory | Description                               |
| ---------------- | ------ | ------------ | ----------------------------------------- |
| `thread_title`   | string | Yes          | The title of the thread to create         |
| `thread_message` | string | Yes          | The content of the thread's first message |

Return:

| Code | Description                                     |
| ---- | ----------------------------------------------- |
| 200  | Thread created successfully                     |
| 404  | Team or channel not found with the given UUIDs  |
| 405  | Channel not found with the given `channel_uuid` |
| 530  | Request failed, user is not logged in           |

##### Thread Context

> Create a new comment/reply in the thread

- Args: `["comment_body"]`
- Server:
  - Send: `CREATE_COMMENT <team_uuid> <channel_uuid> <thread_uuid> "<comment_body>"`
  - Return: `200`/`404`/`405`/`406`/`530`

Args:

| Argument       | Type   | Is Mandatory | Description                          |
| -------------- | ------ | ------------ | ------------------------------------ |
| `comment_body` | string | Yes          | The content of the comment to create |

Return:

| Code | Description                                            |
| ---- | ------------------------------------------------------ |
| 200  | Comment created successfully                           |
| 404  | Team, channel or thread not found with the given UUIDs |
| 405  | Channel not found with the given `channel_uuid`        |
| 406  | Thread not found with the given `thread_uuid`          |
| 530  | Request failed, user is not logged in                  |

#### List (`/list`)

##### Root

> List all existing teams

- Args: None
- Server:
  - Send: `LIST_TEAMS`
  - Return: `200`->`<teams-list>`/`530`

Return:

| Code | Description                                        |
| ---- | -------------------------------------------------- |
| 200  | Success, followed by the teams list (can be empty) |
| 530  | Request failed, user is not logged in              |

##### Team Context

> List all channels of the selected team

- Args: None
- Server:
  - Send: `LIST_CHANNELS <team_uuid>`
  - Return: `200`->`<channels-list>`/`404`/`530`

Return:

| Code | Description                               |
| ---- | ----------------------------------------- |
| 200  | Success, followed by the channels list    |
| 404  | Team not found with the given `team_uuid` |
| 530  | Request failed, user is not logged in     |

##### Channel Context

> List all threads of the selected channel

- Args: None
- Server:
  - Send: `LIST_THREADS <team_uuid> <channel_uuid>`
  - Return: `200`->`<threads-list>`/`404`/`405`/`530`

Return:

| Code | Description                                     |
| ---- | ----------------------------------------------- |
| 200  | Success, followed by the threads list           |
| 404  | Team or channel not found with the given UUIDs  |
| 405  | Channel not found with the given `channel_uuid` |
| 530  | Request failed, user is not logged in           |

##### Thread Context

> List all comments/replies of the selected thread

- Args: None
- Server:
  - Send: `LIST_COMMENTS <team_uuid> <channel_uuid> <thread_uuid>`
  - Return: `200`->`<comments-list>`/`404`/`405`/`406`/`530`

Return:

| Code | Description                                            |
| ---- | ------------------------------------------------------ |
| 200  | Success, followed by the comments list                 |
| 404  | Team, channel or thread not found with the given UUIDs |
| 405  | Channel not found with the given `channel_uuid`        |
| 406  | Thread not found with the given `thread_uuid`          |
| 530  | Request failed, user is not logged in                  |

#### Info (`/info`)

##### Root

> Show details of the currently logged in user

- Args: None
- Server:
  - Send: `INFO_USER`
  - Return: `200`->`<user-info>`/`530`

Return:

| Code | Description                           |
| ---- | ------------------------------------- |
| 200  | Success, followed by the user info    |
| 530  | Request failed, user is not logged in |

##### Team Context

> Show details of the selected team

- Args: None
- Server:
  - Send: `INFO_TEAM <team_uuid>`
  - Return: `200`->`<team-info>`/`404`/`530`

Return:

| Code | Description                               |
| ---- | ----------------------------------------- |
| 200  | Success, followed by the team info        |
| 404  | Team not found with the given `team_uuid` |
| 530  | Request failed, user is not logged in     |

##### Channel Context

> Show details of the selected channel

- Args: None
- Server:
  - Send: `INFO_CHANNEL <team_uuid> <channel_uuid>`
  - Return: `200`->`<channel-info>`/`404`/`405`/`530`

Return:

| Code | Description                                     |
| ---- | ----------------------------------------------- |
| 200  | Success, followed by the channel info           |
| 404  | Team not found with the given `team_uuid`       |
| 405  | Channel not found with the given `channel_uuid` |
| 530  | Request failed, user is not logged in           |

##### Thread Context

> Show details of the selected thread

- Args: None
- Server:
  - Send: `INFO_THREAD <team_uuid> <channel_uuid> <thread_uuid>`
  - Return: `200`->`<thread-info>`/`404`/`405`/`406`/`530`

Return:

| Code | Description                                            |
| ---- | ------------------------------------------------------ |
| 200  | Success, followed by the thread info                   |
| 404  | Team, channel or thread not found with the given UUIDs |
| 405  | Channel not found with the given `channel_uuid`        |
| 406  | Thread not found with the given `thread_uuid`          |
| 530  | Request failed, user is not logged in                  |
