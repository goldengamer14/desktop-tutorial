import { Controller, Get, Post, Param, Query, Body, Put, Delete, ParseIntPipe, UseGuards } from '@nestjs/common';
import { CreateUserDTO } from './dto/create-user.dto';
import { UpdateUserDTO } from './dto/update-user.dto';
import type { User } from './types';
import { UserService } from './user.service';
import { RoleGuard } from 'src/guards/role/role.guard';


@Controller('user')
export class UserController {
    constructor(private readonly userService: UserService) { }

    // Get /user/name?=:name
    @Get()
    getUserByName(@Query("name") name: string): User[] {
        return this.userService.findAllUsers(name);
    }

    // Get user/:id
    @Get(":id")
    getUserById(@Param("id", ParseIntPipe) id: number): User {
        return this.userService.findUserById(id);
    }

    // Post user/
    @Post()
    createUser(@Body() createUserDTO: CreateUserDTO) {
        return this.userService.createUser(createUserDTO);
    }

    // Put user/:id
    @Put(":id")
    @UseGuards(RoleGuard)
    updateUser(@Param("id", ParseIntPipe) id: number, @Body() updateUserDTO: UpdateUserDTO) {
        updateUserDTO.id = id;
        return this.userService.updateUser(updateUserDTO);
    }

    // Delete user/:id
    @Delete(":id")
    @UseGuards(RoleGuard)
    deleteUser(@Param("id", ParseIntPipe) id: number) {
        return this.userService.deleteUserById(id);
    }
}
