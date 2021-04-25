#include "GameClasses.h"

Building::Building(GameData* ptr, ActorType type, Vector2 pos, State state)
    :GameActor(ptr, type, pos, state)
{
    //use GameData to set expansionRange, considering ActorType
    expansionRange = 8; // placeholder, remove later

    //create creep or zerolayer on position when spawned
    ptr->mapExpansionCreep[static_cast<int>(pos.x)][static_cast<int>(pos.y)] = 2;
}

void Building::Expand()
{
    if (!this->expanded)
    {
        std::vector<TileIndex> buf;
    
        for (int i = 1; i <= this->expansionRange; i++)
        {
            buf = this->game->tilesInPerimeterCircle(this->position, i);
            for (TileIndex tile : buf)
            {
                if (this->side == Side::INSECTS)
                {
                    if (this->game->mapExpansionCreep[tile.x][tile.y] != 2)
                    {
                        this->game->mapExpansionCreep[tile.x][tile.y] = 2;
                        return;
                    }
                }
                //TODO: add MACHINES-side expansion
            }
        }
        this->expanded = true;
    }
}
//
//void midPointCircleDraw(int x_centre, int y_centre, int r)
//{
//    int x = r, y = 0;
//
//    // When radius is zero only a single
//    // point will be printed
//    if (r > 0)
//    {
//        cout << "(" << x + x_centre << ", " << -y + y_centre << ") ";
//        cout << "(" << y + x_centre << ", " << x + y_centre << ") ";
//        cout << "(" << -y + x_centre << ", " << x + y_centre << ")\n";
//    }
//
//    // Initialising the value of P
//    int P = 1 - r;
//    while (x > y)
//    {
//        y++;
//
//        // Mid-point is inside or on the perimeter
//        if (P <= 0)
//            P = P + 2 * y + 1;
//        // Mid-point is outside the perimeter
//        else
//        {
//            x--;
//            P = P + 2 * y - 2 * x + 1;
//        }
//
//        // All the perimeter points have already been printed
//        if (x < y)
//            break;
//
//        // Printing the generated point and its reflection
//        // in the other octants after translation
//        cout << "(" << x + x_centre << ", " << y + y_centre << ") ";
//        cout << "(" << -x + x_centre << ", " << y + y_centre << ") ";
//        cout << "(" << x + x_centre << ", " << -y + y_centre << ") ";
//        cout << "(" << -x + x_centre << ", " << -y + y_centre << ")\n";
//
//        // If the generated point is on the line x = y then 
//        // the perimeter points have already been printed
//        if (x != y)
//        {
//            cout << "(" << y + x_centre << ", " << x + y_centre << ") ";
//            cout << "(" << -y + x_centre << ", " << x + y_centre << ") ";
//            cout << "(" << y + x_centre << ", " << -x + y_centre << ") ";
//            cout << "(" << -y + x_centre << ", " << -x + y_centre << ")\n";
//        }
//    }
//}

//для опухолей: проверяет, есть ли в области expansionRange свободное место от слизи (учитывая карту экспансии)
//спавнить за раз 2-4 тайла, каждому свой цикл проверок:
//проходить по спирали радиуса экспансии, проверяя является ли тайл пустым от слизи и имеется ли сосед со слизью

//убываение слизи: раз в n*m циклов проверять все тайлы слизи на наличие рядом источника слизи, если нет - проверять 