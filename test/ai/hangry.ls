var me = Fight.getEntity()
var him = me.getClosestEnemy()

var weapon = me.getWeapons()[0]
System.print(weapon)

var chip = me.getChips()[0]
System.print(chip)

System.print(me.getWeapons())

me.say("Salut !")

me.moveToward(him, 5)

me.setWeapon(weapon)

me.useWeapon(him)

me.moveToward(him, 5)

Field.cell(1, 1)
Field.lineOfSight(me.cell, him.cell)
Field.getPath(me.cell, him.cell)

System.print(Field.getObstacles())
System.print(Field.getObstacles() ~~ x -> x.id)
