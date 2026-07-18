import { IsString, IsEmail, MinLength } from "class-validator";

export class CreateUserDTO {
    @IsString()
    @MinLength(3)
    name!: string;
    @IsEmail()
    email!: string;
};
