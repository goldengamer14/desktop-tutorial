import { CanActivate, ExecutionContext, Injectable, UnauthorizedException } from '@nestjs/common';
import { Request } from 'express';
import { Observable } from 'rxjs';

@Injectable()
export class RoleGuard implements CanActivate {
  canActivate(
    context: ExecutionContext,
  ): boolean {
    const request: Request = context.switchToHttp().getRequest();
    const role = request.headers['role'];

    if (role !== 'admin') 
      throw new UnauthorizedException('You do not have permission to perform this action.');

    return true;
  }
}
