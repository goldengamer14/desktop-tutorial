import { Injectable, NotFoundException } from '@nestjs/common';
import { User } from './types';
import { users } from './constants';
import { LoggerService } from './user.logger';
import { CreateUserDTO } from './dto/create-user.dto';
import { UpdateUserDTO } from './dto/update-user.dto';

@Injectable()
export class UserService {
    constructor(private readonly logger: LoggerService) { }

    private users: User[] = users;
    private usedIds = new Set(users.map(user => user.id));
    private lastId: number = users.length + 1;


    findAllUsers(name: string = ""): User[] {
        if (name) {
            this.logger.log(`Finding Users by Name: ${name}`);
            return this.users.filter(user =>
                user.name.toLowerCase().includes(name.toLowerCase())
            );
        }

        this.logger.log("Finding All Users");
        return this.users;
    }


    findUserById(id: number = 1): User {
        this.logger.log(`Finding User by ID: ${id}`);
        const user: User | undefined = this.users.find(user => user.id == id);

        if (!user) {
            throw new NotFoundException(`User with ID ${id} not found!`)
        }

        return user;
    }

    createUser(createUserDTO: CreateUserDTO): any {
        const newUser: User = {
            id: this.lastId,
            name: createUserDTO.name,
            email: createUserDTO.email
        };

        users.push(newUser);
        this.usedIds.add(this.lastId++);

        const response = {
            user: newUser,
            message: "User created successfully!"
        };

        this.logger.table(response);
        return response;
    }

    updateUser(updateUserDTO: UpdateUserDTO): any {
        if (!this.usedIds.has(updateUserDTO.id!)) {
            throw new NotFoundException(`User with ID ${updateUserDTO.id} not found!`);
        }

        const user: User = users.filter((user: User) => user.id == updateUserDTO.id)[0];
        user.name = updateUserDTO.name ?? user.name;
        user.email = updateUserDTO.email ?? user.email;
        const response = {
            data: user,
            message: "User updated successfully!"
        };

        this.logger.table(response);
        return response;
    }

    deleteUserById(id: number): any {
        this.logger.log("Deleting the user")

        if (!this.usedIds.has(id)) {
            throw new NotFoundException(`User with ID ${id} not found!`);
        }

        let deletedUser: User | undefined = undefined;

        for (let i = 0; i < this.users.length; ++i) {

            if (users[i].id == id) {
                deletedUser = users[i];
                users.splice(i, 1);
                this.usedIds.delete(i + 1);
            }
        }
        const response = {
            data: deletedUser,
            message: (deletedUser
                ? "User deleted sucessfully!"
                : "User Not Found!")
        };

        this.logger.table(response);
        return response;
    }
}
