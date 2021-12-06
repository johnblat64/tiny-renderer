
// void R_drawTriangle(v3d *vs, SDL_Color color){
//     Uint32 color32 = SDL_MapRGB(gWindowSurface->format, color.r, color.g, color.b);
//     //sort
//     v2d bboxMin = {
//         (float)gCanvasDimensions.x,
//         (float)gCanvasDimensions.y
//     };

//     v2d v2ds[3];
//     for(int i = 0; i < 3; i++){
//         v2ds[i].x = vs[i].x;
//         v2ds[i].y = vs[i].y;
//     }

//     v2d bboxMax = {0, 0};
//     v2d clamp = {(float)gCanvasDimensions.x, (float)gCanvasDimensions.y};
    
//     for(int i = 0; i < 3; i++){
//         for(int j = 0; j < 2; j++){
//             bboxMin[j] = max(0, min(bboxMin[j], vs[i][j]));
//             bboxMax[j] = min(clamp[j], max(bboxMax[j], vs[i][j]));
//         }
//     }

//     v3d P = {0, 0, 0};
  
//     for(P.y = (int)bboxMin.y; P.y <= (int)bboxMax.y; P.y++){
//         for(P.x = (int)bboxMin.x; P.x <= (int)bboxMax.x; P.x++){
//             v3d bcScreen = baycentric(v2ds, v2d(P.x, P.y));
            
//             if(bcScreen.x < 0.0f || bcScreen.y < 0.0f || bcScreen.z < 0.0f){
//                 continue;
//             }
//             P.z = 0;
//             for(int i = 0; i < 3; i++){
//                 P.z += vs[i].z * bcScreen[i];
//             }
//             if(g_zBuffer[int(P.x + P.y * gCanvasDimensions.x)] <= P.z){
//                 g_zBuffer[int(P.x + P.y * gCanvasDimensions.x)] = P.z;
//                 R_putPixel(
//                     (v2d){P.x, P.y},
//                     color32
//                 );
//             }
//             else {
//                 printf("not!\n");
//             }
            
//                 // SDL_Delay(50);
//                 // SDL_UpdateWindowSurface(gWindow);
            
                
//         }
//     }
    
// }

// void R_drawTriangle2(v2d *vs, SDL_Color color){
//     if(vs[0].y==vs[1].y && vs[0].y == vs[2].y){
//         return;
//     }

//     if(vs[0].y > vs[1].y){
//         swapValues(vs[0], vs[1], v2d);
//     }
//     if(vs[0].y > vs[2].y){
//         swapValues(vs[0], vs[2], v2d);
//     }
//     if(vs[1].y > vs[2].y){
//         swapValues(vs[1], vs[2], v2d);
//     }

//     int totalHeight = vs[2].y - vs[0].y;

//     for(int i = 0; i < totalHeight; i++){
//         bool secondHalf = i > vs[1].y - vs[0].y || vs[1].y == vs[0].y;
//         int segmentHeight = secondHalf ? vs[2].y - vs[1].y : vs[1].y - vs[0].y;
//         float alpha = (float)i/(float)totalHeight;
//         float beta = (float)(i-(secondHalf ? vs[1].y - vs[0].y : 0)) / (float)segmentHeight;
        
//         v2d A = v2d_add(vs[0], v2d_scale(alpha, v2d_sub(vs[2], vs[0])));
//         v2d B = secondHalf ? v2d_add(vs[1], v2d_scale(beta, v2d_sub(vs[2],vs[1]))) : v2d_add(vs[0], v2d_scale( beta, v2d_sub(vs[1], vs[0])));

//         if(A.x>B.x){
//             swapValues(A, B, v2d);
//         }
//         for(int j = A.x; j <= B.x; j++){
//             R_putPixel(
//                 (v2d){
//                     (float)j,
//                     vs[0].y+i
//                 },
//                 color
//             );
//              SDL_Delay(10);
//                 SDL_UpdateWindowSurface(gWindow);
//         }
//     }

// }


// Matrix v2m(v3d v){
//     Matrix m(4, 1);
//     m[0][0] = v.x;
//     m[1][0] = v.y;
//     m[2][0] = v.z;
//     m[3][0] = 1.0f;

//     return m;
// }

// v3d m2v(Matrix m){
//     v3d v(
//         m[0][0]/m[3][0],
//         m[1][0]/m[3][0],
//         m[2][0]/m[3][0]
//     );
//     return v;
// }


typedef struct Line2d {
    v2d v0, v1;
} Line2d;

typedef struct Line3d {
    v2d v0, v1, v2;
} Line3d;



// void R_drawLineSolid(Line2d line, SDL_Color color){
//     Uint32 color32 = SDL_MapRGB(gWindowSurface->format, color.r, color.g, color.b);
//     SDL_bool steep = SDL_FALSE;

//     if(fabsf(line.v0.x - line.v1.x) < fabsf(line.v0.y - line.v1.y)){
//         swapValues(line.v0.x, line.v0.y, float);
//         swapValues(line.v1.x, line.v1.y, float);
//         steep = SDL_TRUE;
//     }
//     if(line.v0.x > line.v1.x){
//         swapValues(line.v0.x, line.v1.x, float);
//         swapValues(line.v0.y, line.v1.y, float);
//     }
//     int dx = line.v1.x - line.v0.x;
//     int dy = line.v1.y - line.v0.y;
//     int derror2 = abs(dy)*2;
//     int error2 = 0;
//     int y = line.v0.y;
//     for(int x = line.v0.x; x <= line.v1.x; x++){
//         if(steep){
//             R_putPixel(
//                 (v2d){
//                     (float)y,
//                     (float)x
//                 },
//                 color32
//             );
//         }
//         else {
//             R_putPixel(
//                 (v2d){
//                     (float)x,
//                     (float)y
//                 },
//                 color32
//             );
//         }
//         error2 += derror2;
//         if(error2 > dx) {
//             y += (line.v1.y > line.v0.y ? 1 : -1);
//             error2 -= dx * 2;
//         }
//     }
// }

// BARYCENTRIC
    // v3d pt0 = {
    //     pts[2].x-pts[0].x,
    //     pts[1].x-pts[0].x,
    //     pts[0].x-P.x
    // };
    // v3d pt1 = {
    //     pts[2].y-pts[0].y,
    //     pts[1].y-pts[0].y,
    //     pts[0].y-P.y
    // };
    // v3d u = v3d_cross(pt0, pt1);
    // if(fabsf(u.z) < 1.0f){
    //     return (v3d){-1, 1, 1};
    // }
    // return (v3d){
    //     1.0f-(u.x+u.y)/u.z,
    //     u.y/u.z,
    //     u.x/u.z
    // };
    // v2d pts2 = pts;
    // pts