# Tutorial to create your own radio on a new vehicle

This tutorial describes how to add a radio to your vehicle (You need a basic skill of working in Reforger Tools).

Let's take a [Big Chungus Motorbike (61B84088181A8FA4)](https://reforger.armaplatform.com/workshop/61B84088181A8FA4-BigChungusMotorbike) as an example.

<picture><img src="./images_veh/1.png" alt="image-1.png" style="width:auto; height: 300px"></picture>

> [!IMPORTANT]
> If your vehicle is not inherited from Vehicle_Base.et see [Not Common Vehicle section](#not-common-vehicle).

In your vehicle prefab find a SlotManagerComponment, then find CustomRadio Slot and **Enable** it, afterwards **Position** (and **Rotate**) it to where you want.

> [!IMPORTANT]
> It is better to set Pivot ID to a part of the model on which the radio will be attached. Otherwise you will get what is described in [Troubleshooting section](#troubleshooting)

<picture><img src="./images_veh/2.png" alt="image-2.png" style="width:auto; height: 300px"></picture>
<picture><img src="./images_veh/3.png" alt="image-3.png" style="width:auto; height: 300px"></picture>

Then find ActionsManagerComponent and find CustomRadio context. Then **Position** it where you want.

<picture><img src="./images_veh/4.png" alt="image-4.png" style="width:auto; height: 300px"></picture>
<picture><img src="./images_veh/5.png" alt="image-5.png" style="width:auto; height: 300px"></picture>

And rotate it as you want. You can enable **context visibility angle visualization** to better understand the direction of the action field of view. 

<picture><img src="./images_veh/6.png" alt="image-6.png" style="width:auto; height: 300px"></picture>
<picture><img src="./images_veh/7.png" alt="image-7.png" style="width:auto; height: 300px"></picture>
<picture><img src="./images_veh/8.png" alt="image-8.png" style="width:auto; height: 300px"></picture>

And then you can test it.

<picture><img src="./images_veh/9.png" alt="image-9.png" style="width:auto; height: 300px"></picture>


## Not Common Vehicle 

If your vehicle isn't inherited from Vehicle_Base.et you will need to copy slot and actions to your vehicle.

Let's take a boat from [Usable boats (999000999BE11000)](https://reforger.armaplatform.com/workshop/999000999BE11000-Usableboats) as an example.

<picture><img src="./images_veh/10.png" alt="image-10.png" style="width:auto; height: 300px"></picture>

Find Vehicle_Base.et prefab and add it to a world to copy values.

<picture><img src="./images_veh/11.png" alt="image-11.png" style="width:auto; height: 300px"></picture>

Add a new EntitySlotInfo to your vehicle's SlotManagerComponment and copy values from Vehicle_Base.et

<picture><img src="./images_veh/12.png" alt="image-12.png" style="width:auto; height: 300px"></picture>

Then copy context and actions from ActionsManagerComponent of Vehicle_Base.et to your prefab's ActionsManagerComponent.

> [!TIP]
> Create actions in the same order to make it look like ordinary vehicles.
> 
<picture><img src="./images_veh/13.png" alt="image-13.png" style="width:auto; height: 300px"></picture>

Then position slot and actions context in the same way as in the previous section.

<picture><img src="./images_veh/14.png" alt="image-14.png" style="width:auto; height: 300px"></picture>

<picture><img src="./images_veh/15.png" alt="image-15.png" style="width:auto; height: 300px"></picture>
<picture><img src="./images_veh/16.png" alt="image-16.png" style="width:auto; height: 300px"></picture>


## Troubleshooting

If you see something like this, you need to position CustomRadio slot and CustomRadio action context relative to a part of a vehicle. 

<picture><img src="./images_veh/t-1.png" alt="image-t-1.png" style="width:auto; height: 300px"></picture>

You need to set Pivot ID and position them relative to this part. 

<picture><img src="./images_veh/t-2.png" alt="image-t-2.png" style="width:auto; height: 100px"></picture>

<picture><img src="./images_veh/t-3.png" alt="image-t-2.png" style="width:auto; height: 300px"></picture>
