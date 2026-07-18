import { Injectable, NestMiddleware, UnauthorizedException } from '@nestjs/common';
import { NextFunction, Request, Response } from 'express';
import { APIKEY } from './constants';

@Injectable()
export class ApiKeyMiddleware implements NestMiddleware {
  use(req: Request, res: Response, next: NextFunction) {
    const apiKey: string | string[] | undefined = req.headers['x-api-key'];
    const CUD: boolean = req.method.toLowerCase() !== "get";

    console.log("ApiKeyMiddleware: Received API key:", apiKey);

    if (CUD)
      if (!apiKey || apiKey !== APIKEY) {
        throw new UnauthorizedException('Invalid API key');
      }

    next();
  }
}
